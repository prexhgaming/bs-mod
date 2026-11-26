package zyn.bs.view.game

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import zyn.bs.data.GameRepository

/** Wires the repository into [GameViewModel] without a DI framework. */
class GameFactory(private val repository: GameRepository) : ViewModelProvider.Factory {

    @Suppress("UNCHECKED_CAST")
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        require(modelClass.isAssignableFrom(GameViewModel::class.java)) {
            "Unknown ViewModel class: ${modelClass.name}"
        }
        return GameViewModel(repository) as T
    }
}
