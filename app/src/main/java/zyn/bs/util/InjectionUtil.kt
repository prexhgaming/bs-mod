package zyn.bs.util

import zyn.bs.data.GameRepository
import zyn.bs.view.game.GameFactory

/**
 * Minimal dependency wiring: the app has exactly one repository and one screen.
 */
object InjectionUtil {

    private val gameRepository = GameRepository()

    fun getGameFactory(): GameFactory {
        return GameFactory(gameRepository)
    }
}
