# macOS .app bundle assembly for Starship.
#
# Builds Starship.app directly from the normal build (MACOSX_BUNDLE), generates the
# app icon, bundles the runtime resources the game and the first-run asset extractor
# need into Contents/Resources, relinks non-system dylibs into Contents/Frameworks,
# and ad-hoc codesigns the result so it launches without "damaged app" warnings.
#
# Resource layout the runtime expects on macOS:
#   * Ship::Context::GetAppBundlePath()  -> <App>.app/Contents/Resources  (read-only)
#       - starship.o2r        : packed port assets (GeneratePortO2R target)
#       - config.yml, assets/ : asset definitions read by the first-run ROM extractor
#       - gamecontrollerdb.txt
#   * Ship::Context::GetAppDirectoryPath() -> SHIP_HOME (~/Library/Application Support/com.starship)
#       - sf64.o2r is extracted here on first run, alongside config/saves/mods

set(MACOS_DIR ${CMAKE_SOURCE_DIR}/cmake/macos)
set(ENTITLEMENTS_FILE ${MACOS_DIR}/entitlements.plist)

option(STARSHIP_BUNDLE_DEPS "Relink and bundle dylibs into the .app so it is portable" ON)

# ---------------------------------------------------------------------------
# Bundle metadata
# ---------------------------------------------------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
    XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS ${ENTITLEMENTS_FILE}
)

# ---------------------------------------------------------------------------
# App icon: Starship.icns generated from logo.png (matches CFBundleIconFile).
# macOS rounds the square automatically, so it reads as a native icon.
# ---------------------------------------------------------------------------
set(ICON_SRC ${CMAKE_SOURCE_DIR}/logo.png)
set(ICONSET_DIR ${CMAKE_BINARY_DIR}/macosx/Starship.iconset)
set(ICNS_FILE ${CMAKE_BINARY_DIR}/macosx/Starship.icns)
add_custom_command(
    OUTPUT ${ICNS_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ICONSET_DIR}
    COMMAND sips -z 16 16     ${ICON_SRC} --out ${ICONSET_DIR}/icon_16x16.png
    COMMAND sips -z 32 32     ${ICON_SRC} --out ${ICONSET_DIR}/icon_16x16@2x.png
    COMMAND sips -z 32 32     ${ICON_SRC} --out ${ICONSET_DIR}/icon_32x32.png
    COMMAND sips -z 64 64     ${ICON_SRC} --out ${ICONSET_DIR}/icon_32x32@2x.png
    COMMAND sips -z 128 128   ${ICON_SRC} --out ${ICONSET_DIR}/icon_128x128.png
    COMMAND sips -z 256 256   ${ICON_SRC} --out ${ICONSET_DIR}/icon_128x128@2x.png
    COMMAND sips -z 256 256   ${ICON_SRC} --out ${ICONSET_DIR}/icon_256x256.png
    COMMAND sips -z 512 512   ${ICON_SRC} --out ${ICONSET_DIR}/icon_256x256@2x.png
    COMMAND sips -z 512 512   ${ICON_SRC} --out ${ICONSET_DIR}/icon_512x512.png
    COMMAND sips -z 1024 1024 ${ICON_SRC} --out ${ICONSET_DIR}/icon_512x512@2x.png
    COMMAND iconutil -c icns -o ${ICNS_FILE} ${ICONSET_DIR}
    DEPENDS ${ICON_SRC}
    COMMENT "Generating Starship.icns from ${ICON_SRC}"
    VERBATIM
)
add_custom_target(StarshipIcon DEPENDS ${ICNS_FILE})
add_dependencies(${PROJECT_NAME} StarshipIcon)

# Ensure the packed port assets (starship.o2r) are generated before the app links,
# so the POST_BUILD step below always has them to copy into the bundle. (sf64.o2r is
# created at runtime from the user's ROM into SHIP_HOME, so it is intentionally not
# a build dependency.)
add_dependencies(${PROJECT_NAME} GeneratePortO2R)
set_source_files_properties(${ICNS_FILE} PROPERTIES GENERATED TRUE MACOSX_PACKAGE_LOCATION "Resources")
target_sources(${PROJECT_NAME} PRIVATE ${ICNS_FILE})

# ---------------------------------------------------------------------------
# Copy runtime resources into Contents/Resources after the app links
# ---------------------------------------------------------------------------
set(RES_DIR "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Resources")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/config.yml" "${RES_DIR}/config.yml"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${RES_DIR}/assets"
    # starship.o2r and gamecontrollerdb.txt are generated/downloaded; copy if present.
    COMMAND bash -c "[ -f '${CMAKE_SOURCE_DIR}/starship.o2r' ] && cp '${CMAKE_SOURCE_DIR}/starship.o2r' '${RES_DIR}/starship.o2r' || echo 'note: starship.o2r not found yet - build the GeneratePortO2R target, then rebuild'"
    COMMAND bash -c "[ -f '${CMAKE_BINARY_DIR}/gamecontrollerdb.txt' ] && cp '${CMAKE_BINARY_DIR}/gamecontrollerdb.txt' '${RES_DIR}/gamecontrollerdb.txt' || true"
    COMMENT "Bundling Starship resources into the .app"
    VERBATIM
)

# ---------------------------------------------------------------------------
# Relink dylibs into Contents/Frameworks (portable .app) and codesign
# ---------------------------------------------------------------------------
if (STARSHIP_BUNDLE_DEPS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -DAPP_BUNDLE=$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>
            -DEXECUTABLE_NAME=${PROJECT_NAME}
            -P ${MACOS_DIR}/fixup_bundle.cmake
        COMMAND bash -c "install_name_tool -add_rpath '@executable_path/../Frameworks/' '$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/MacOS/${PROJECT_NAME}' 2>/dev/null || true"
        COMMENT "Relinking dylibs into the .app bundle"
        VERBATIM
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND codesign --force --deep --sign - --options runtime --entitlements ${ENTITLEMENTS_FILE} "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
    COMMENT "Ad-hoc codesigning Starship.app"
    VERBATIM
)

# For `cpack` / `make install`, install the assembled bundle.
install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION . COMPONENT ${PROJECT_NAME})
