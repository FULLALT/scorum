#include <boost/test/unit_test.hpp>
#include "budget_check_common.hpp"

namespace budget_check_common {

budget_check_fixture::budget_check_fixture()
    : post_budget_service(db.post_budget_service())
    , banner_budget_service(db.banner_budget_service())
{
}

std::string budget_check_fixture::get_unique_permlink()
{
    return boost::lexical_cast<std::string>(++permlink_no);
}

create_budget_operation
budget_check_fixture::create_budget(const uuid_type& uuid, const Actor& owner, const budget_type type)
{
    return create_budget(uuid, owner, type, BUDGET_BALANCE_DEFAULT, BUDGET_DEADLINE_IN_BLOCKS_DEFAULT);
}

create_budget_operation budget_check_fixture::create_budget(
    const uuid_type& uuid, const Actor& owner, const budget_type type, int balance, uint32_t deadline_blocks_offset)
{
    // start_time should be greater than head_block_time
    return create_budget(uuid, owner, type, balance, 1, deadline_blocks_offset);
}

create_budget_operation budget_check_fixture::create_budget(const uuid_type& uuid,
                                                            const Actor& owner,
                                                            const budget_type type,
                                                            int balance,
                                                            uint32_t start_blocks_offset,
                                                            uint32_t deadline_blocks_offset)
{
    create_budget_operation op;
    op.uuid = uuid;
    op.owner = owner.name;
    op.type = type;
    op.balance = asset(balance, SCORUM_SYMBOL);
    op.start = db.head_block_time() + start_blocks_offset * SCORUM_BLOCK_INTERVAL;
    op.deadline = db.head_block_time() + deadline_blocks_offset * SCORUM_BLOCK_INTERVAL;
    op.json_metadata = get_unique_permlink();

    push_operation_only(op, owner.private_key);

    return op;
}

create_budget_operation budget_check_fixture::create_budget(const uuid_type& uuid,
                                                            const Actor& owner,
                                                            const budget_type type,
                                                            const asset& balance,
                                                            const fc::time_point_sec& start,
                                                            const fc::time_point_sec& deadline)
{
    create_budget_operation op;
    op.uuid = uuid;
    op.owner = owner.name;
    op.type = type;
    op.balance = balance;
    op.start = start;
    op.deadline = deadline;
    op.json_metadata = get_unique_permlink();

    push_operation_only(op, owner.private_key);

    generate_block();

    return op;
}

} // namespace database_fixture
