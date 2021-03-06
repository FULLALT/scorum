#pragma once

#include <scorum/chain/services/service_base.hpp>
#include <scorum/chain/schema/witness_objects.hpp>

namespace scorum {
namespace protocol {
struct chain_properties;
}
namespace chain {

namespace dba {
template <typename> class db_accessor;
}

class account_object;

using chain_properties = scorum::protocol::chain_properties;

struct witness_service_i : public base_service_i<witness_object>
{
    using base_service_i<witness_object>::get;
    using base_service_i<witness_object>::is_exists;

    virtual const witness_object& get(const account_name_type& owner) const = 0;

    virtual bool is_exists(const account_name_type& owner) const = 0;

    virtual const witness_object& get_top_witness() const = 0;

    virtual const witness_object& create_witness(const account_name_type& owner,
                                                 const std::string& url,
                                                 const public_key_type& block_signing_key,
                                                 const chain_properties& props)
        = 0;

    virtual const witness_object& create_initial_witness(const account_name_type& owner,
                                                         const public_key_type& block_signing_key)
        = 0;

    virtual void update_witness(const witness_object& witness,
                                const std::string& url,
                                const public_key_type& block_signing_key,
                                const chain_properties& props)
        = 0;

    /** this updates the vote of a single witness as a result of a vote being added or removed*/
    virtual void adjust_witness_vote(const witness_object& witness, const share_type& delta) = 0;

    /** this is called by `adjust_proxied_witness_votes` when account proxy to self */
    virtual void adjust_witness_votes(const account_object& account, const share_type& delta) = 0;
};

class dbs_witness : public dbs_service_base<witness_service_i>
{
    friend class dbservice_dbs_factory;

public:
    explicit dbs_witness(dba::db_index& db,
                         witness_schedule_service_i&,
                         dynamic_global_property_service_i&,
                         dba::db_accessor<chain_property_object>&);

    using base_service_i<witness_object>::get;
    using base_service_i<witness_object>::is_exists;

    const witness_object& get(const account_name_type& owner) const override;

    bool is_exists(const account_name_type& owner) const override;

    const witness_object& get_top_witness() const override;

    const witness_object& create_witness(const account_name_type& owner,
                                         const std::string& url,
                                         const public_key_type& block_signing_key,
                                         const chain_properties& props) override;

    const witness_object& create_initial_witness(const account_name_type& owner,
                                                 const public_key_type& block_signing_key) override;

    void update_witness(const witness_object& witness,
                        const std::string& url,
                        const public_key_type& block_signing_key,
                        const chain_properties& props) override;

    /** this updates the vote of a single witness as a result of a vote being added or removed*/
    void adjust_witness_vote(const witness_object& witness, const share_type& delta) override;

    /** this is called by `adjust_proxied_witness_votes` when account proxy to self */
    void adjust_witness_votes(const account_object& account, const share_type& delta) override;

private:
    const witness_object& create_internal(const account_name_type& owner, const public_key_type& block_signing_key);
    block_info get_head_block_context();

    dynamic_global_property_service_i& _dgp_svc;
    witness_schedule_service_i& _witness_schedule_svc;
    dba::db_accessor<chain_property_object>& _chain_dba;
};
} // namespace chain
} // namespace scorum
