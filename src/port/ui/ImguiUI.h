#pragma once
#include <libultraship/libultraship.h>

namespace GameUI {
    const std::string LevelNameLookup[] = 
      { 
        "Corneria", "Meteo", "Sector X", "Area 6", 
        "Unknown", "Sector Y", "Venom 1", "Solar", 
        "Zoness", "Andross", "Training", "Macbeth", 
        "Titania", "Aquas", "Fortuna", "Unknown", 
        "Katina", "Bolse", "Sector Z", "Venom 2", 
        "Versus"
      };
    void SetupGuiElements();
    void Destroy();
}

class GameMenuBar : public Ship::GuiMenuBar {
  public:
    using Ship::GuiMenuBar::GuiMenuBar;
  protected:
    void DrawElement() override;
    void InitElement() override {};
    void UpdateElement() override {};
};