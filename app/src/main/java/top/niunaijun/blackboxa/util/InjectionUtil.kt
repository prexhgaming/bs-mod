package top.niunaijun.blackboxa.util

import top.niunaijun.blackboxa.data.GameRepository
import top.niunaijun.blackboxa.view.game.GameFactory

/**
 * Minimal dependency wiring: the app has exactly one repository and one screen.
 */
object InjectionUtil {

    private val gameRepository = GameRepository()

    fun getGameFactory(): GameFactory {
        return GameFactory(gameRepository)
    }
}
