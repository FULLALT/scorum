#ifdef IS_TEST_NET
#include <boost/test/unit_test.hpp>

#include <scorum/chain/scorum_objects.hpp>
#include <scorum/chain/dbs_registration_pool.hpp>

#include "database_fixture.hpp"

using namespace scorum;
using namespace scorum::chain;
using namespace scorum::protocol;

//
// usage for all registration tests 'chain_test  -t registration_*'
//

class registration_pool_service_check_fixture : public timed_blocks_database_fixture
{
public:
    registration_pool_service_check_fixture()
        : timed_blocks_database_fixture(test::create_registration_genesis()),
          registration_pool_service(db.obtain_service<dbs_registration_pool>())
    {
    }

    dbs_registration_pool& registration_pool_service;

    const std::string genesis_invalid_schedule_users_thousands_str = R"json(
    {
            "registration_supply": 9999,
            "registration_maximum_bonus": 5,
            "registration_schedule": [
            {
                    "stage": 1,
                    "users_thousands": 0,
                    "bonus_percent": 100
            }]
    })json";

    const std::string genesis_invalid_schedule_bonus_percent_l_str = R"json(
    {
            "registration_supply": 9999,
            "registration_maximum_bonus": 5,
            "registration_schedule": [
            {
                    "stage": 1,
                    "users_thousands": 1,
                    "bonus_percent": -10
            }]
    })json";

    const std::string genesis_invalid_schedule_bonus_percent_h_str = R"json(
    {
            "registration_supply": 9999,
            "registration_maximum_bonus": 5,
            "registration_schedule": [
            {
                    "stage": 1,
                    "users_thousands": 1,
                    "bonus_percent": 110
            }]
    })json";
};

BOOST_FIXTURE_TEST_SUITE(registration_pool_service_input_check, registration_pool_service_check_fixture)

SCORUM_TEST_CASE(create_invalid_genesis_state_amount_check)
{
    genesis_state_type invalid_genesis_state = genesis_state;
    invalid_genesis_state.registration_supply = 0;

    BOOST_CHECK_THROW(registration_pool_service.create_pool(invalid_genesis_state), fc::assert_exception);

    invalid_genesis_state = genesis_state;
    invalid_genesis_state.registration_maximum_bonus = 0;

    BOOST_CHECK_THROW(registration_pool_service.create_pool(invalid_genesis_state), fc::assert_exception);

    invalid_genesis_state = genesis_state;
    invalid_genesis_state.registration_schedule.clear();

    BOOST_CHECK_THROW(registration_pool_service.create_pool(invalid_genesis_state), fc::assert_exception);
}

SCORUM_TEST_CASE(create_invalid_genesis_schedule_schedule_check)
{
    genesis_state_type invalid_genesis_state;

    invalid_genesis_state = fc::json::from_string(genesis_invalid_schedule_users_thousands_str).as<genesis_state_type>();

    BOOST_CHECK_THROW(registration_pool_service.create_pool(invalid_genesis_state), fc::assert_exception);

    invalid_genesis_state = fc::json::from_string(genesis_invalid_schedule_bonus_percent_l_str).as<genesis_state_type>();

    BOOST_CHECK_THROW(registration_pool_service.create_pool(invalid_genesis_state), fc::assert_exception);

    invalid_genesis_state = fc::json::from_string(genesis_invalid_schedule_bonus_percent_h_str).as<genesis_state_type>();

    BOOST_CHECK_THROW(registration_pool_service.create_pool(invalid_genesis_state), fc::assert_exception);
}

SCORUM_TEST_CASE(create_check)
{
    if (!registration_pool_service.is_pool_exists())
    {
        //if pool has not created in basic fixture
        BOOST_REQUIRE_NO_THROW(registration_pool_service.create_pool(genesis_state));
    }

    const registration_pool_object& pool = registration_pool_service.get_pool();

    BOOST_CHECK_EQUAL(pool.balance.amount.value, 9999);
    BOOST_CHECK_EQUAL(pool.maximum_bonus.value, 5);
    BOOST_CHECK_EQUAL(pool.already_allocated_count, 0);

    BOOST_REQUIRE_EQUAL(pool.schedule_items.size(), 4);

    int inpit[][2] = {{100, 100}, {200, 75}, {300, 50}, {400, 25}};
    std::size_t ci = 0;
    for (const auto &item: pool.schedule_items)
    {
        BOOST_CHECK_EQUAL(item.users_thousands, inpit[ci][0]);
        BOOST_CHECK_EQUAL(item.bonus_percent, inpit[ci][1]);

        ++ci;
    }
}

SCORUM_TEST_CASE(create_double_check)
{
    if (!registration_pool_service.is_pool_exists())
    {
        //if pool has not created in basic fixture
        BOOST_REQUIRE_NO_THROW(registration_pool_service.create_pool(genesis_state));
    }

    BOOST_REQUIRE_THROW(registration_pool_service.create_pool(genesis_state), fc::assert_exception);
}

SCORUM_TEST_CASE(allocate_cash_check)
{
    //TODO
}

BOOST_AUTO_TEST_SUITE_END()

#endif
