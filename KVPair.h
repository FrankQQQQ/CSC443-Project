#ifndef KV_STORE_KVPAIR_H
#define KV_STORE_KVPAIR_H

#include <string>
using std::string;

class KVPair {
public:
    KVPair(const string &key, const string &value);

    const string &getKey() const;
    void setKey(const string &key);
    const string &getValue() const;
    void setValue(const string &value);
private:
    string key;
    string value;
};

#endif //KV_STORE_KVPAIR_H
