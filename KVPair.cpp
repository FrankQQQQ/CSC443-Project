#include "KVPair.h"

const string &KVPair::getKey() const {
    return key;
}

void KVPair::setKey(const string &key) {
    KVPair::key = key;
}

const string &KVPair::getValue() const {
    return value;
}

void KVPair::setValue(const string &value) {
    KVPair::value = value;
}

KVPair::KVPair(const string &key, const string &value) {
    setKey(key);
    setValue(value);
}
