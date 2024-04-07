#include "GenericArray.h"

namespace SF64 {
GenericArrayData* GenericArray::GetPointer() {
    return mData.data();
}

size_t GenericArray::GetPointerSize() {
    return sizeof(mData);
}
}