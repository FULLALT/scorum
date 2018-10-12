#pragma once

#define API_BETTING "betting_api"

#include <memory>

#include <scorum/protocol/types.hpp>
#include <scorum/app/api.hpp>
#include <scorum/app/betting_api_objects.hpp>

namespace chainbase {
class database_guard;
}

namespace scorum {
namespace app {

class betting_api_impl;
struct api_context;

/**
 * @brief Betting API
 *
 * This api does not require plugins
 *
 * @ingroup api
 * @addtogroup betting_api Betting API
 */
class betting_api : public std::enable_shared_from_this<betting_api>
{
public:
    betting_api(const api_context& ctx);

    void on_api_startup();

    /// @name Public API
    /// @addtogroup betting_api
    /// @{

    /**
     * @brief Returns all winners for particular game
     * @param game_uuid Game UUID
     * @return array of winner_api_object's
     */
    std::vector<winner_api_object> get_game_winners(const uuid_type& game_uuid) const;

    /**
     * @brief Returns games
     * @param filter [created, started, finished]
     * @return array of game_api_object's
     */
    std::vector<game_api_object> get_games(game_filter filter) const;

    /**
     * @brief Returns matched bets
     * @param from lower bound bet id
     * @param limit query limit
     * @return array of matched_bet_api_object's
     */
    std::vector<matched_bet_api_object> get_matched_bets(matched_bet_id_type from, uint32_t limit) const;

    /**
     * @brief Return pending bets
     * @param from lower bound bet id
     * @param limit query limit
     * @return array of pending_bet_api_object's
     */
    std::vector<pending_bet_api_object> get_pending_bets(pending_bet_id_type from, uint32_t limit) const;

    /**
     * @brief Return betting properties
     * @return betting propery api object
     */
    betting_property_api_object get_betting_properties() const;

    /// @}

private:
    std::unique_ptr<betting_api_impl> _impl;

    std::shared_ptr<chainbase::database_guard> _guard;
};

} // namespace app
} // namespace scorum

// clang-format off
FC_API(scorum::app::betting_api, (get_game_winners)
                                 (get_games)
                                 (get_matched_bets)
                                 (get_pending_bets)
                                 (get_betting_properties))
// clang-format on