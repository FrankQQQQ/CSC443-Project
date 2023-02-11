#ifndef KV_STORE_KVPAIR_H
#define KV_STORE_KVPAIR_H

#include <string>
#include <ostream>
using std::string;

class KVPair {
public:
    KVPair(const string &key, const string &value);

    bool operator==(const KVPair &other) const;
    bool operator<(const KVPair &other) const;

    const string &getKey() const;
    void setKey(const string &key);
    const string &getValue() const;
    void setValue(const string &value);
private:
    string key;
    string value;
};

std::ostream &operator<<(std::ostream &stream, const KVPair &kvPair);

#endif //KV_STORE_KVPAIR_H
