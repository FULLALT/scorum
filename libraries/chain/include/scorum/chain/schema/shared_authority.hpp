#pragma once
#include <scorum/protocol/authority.hpp>
#include <fc/shared_containers.hpp>

namespace scorum {
namespace chain {
using scorum::protocol::authority;
using scorum::protocol::public_key_type;
using scorum::protocol::account_name_type;
using scorum::protocol::authority_weight_type;

/**
 *  The purpose of this class is to represent an authority object in a manner compatible with
 *  shared memory storage.  This requires all dynamic fields to be allocated with the same allocator
 *  that allocated the shared_authority.
 */
struct shared_authority
{
private:
    shared_authority() = delete;

public:
    template <typename Allocator>
    shared_authority(const authority& a, const Allocator& alloc)
        : account_auths(alloc)
        , key_auths(alloc)
    {
        account_auths.reserve(a.account_auths.size());
        key_auths.reserve(a.key_auths.size());
        for (const auto& item : a.account_auths)
            account_auths.insert(item);
        for (const auto& item : a.key_auths)
            key_auths.insert(item);
        weight_threshold = a.weight_threshold;
    }

    shared_authority(const shared_authority& cpy)
        : weight_threshold(cpy.weight_threshold)
        , account_auths(cpy.account_auths)
        , key_auths(cpy.key_auths)
    {
    }

    template <typename Allocator>
    shared_authority(const Allocator& alloc)
        : account_auths(alloc)
        , key_auths(alloc)
    {
    }

    template <typename Allocator, class... Args>
    shared_authority(const Allocator& alloc, uint32_t weight_threshold, Args... auths)
        : weight_threshold(weight_threshold)
        , account_auths(alloc)
        , key_auths(alloc)
    {
        add_authorities(auths...);
    }

    operator authority() const;

    shared_authority& operator=(const authority& a);

    void add_authority(const public_key_type& k, authority_weight_type w);
    void add_authority(const account_name_type& k, authority_weight_type w);

    template <typename AuthType> void add_authorities(AuthType k, authority_weight_type w)
    {
        add_authority(k, w);
    }

    template <typename AuthType, class... Args> void add_authorities(AuthType k, authority_weight_type w, Args... auths)
    {
        add_authority(k, w);
        add_authorities(auths...);
    }

    std::vector<public_key_type> get_keys() const;

    bool is_impossible() const;
    uint32_t num_auths() const;
    void clear();
    void validate() const;

    typedef fc::shared_flat_map<account_name_type, authority_weight_type> account_authority_map;
    typedef fc::shared_flat_map<public_key_type, authority_weight_type> key_authority_map;

    uint32_t weight_threshold = 0;
    account_authority_map account_auths;
    key_authority_map key_auths;
};

bool operator==(const shared_authority& a, const shared_authority& b);
bool operator==(const authority& a, const shared_authority& b);
bool operator==(const shared_authority& a, const authority& b);
}
} // scorum::chain

FC_REFLECT_TYPENAME(scorum::chain::shared_authority::account_authority_map)
FC_REFLECT(scorum::chain::shared_authority, (weight_threshold)(account_auths)(key_auths))
