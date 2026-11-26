package top.niunaijun.blackboxa.util

import android.app.Activity
import android.app.Dialog
import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.viewbinding.ViewBinding

/**
 * ViewBinding delegate used by the launcher and the crash viewer.
 *
 * The generated binding classes are found reflectively, so R8 must keep their
 * static inflate(...) methods — see proguard-rules.pro.
 */
inline fun <reified T : ViewBinding> Activity.inflate(): Lazy<T> = lazy {
    inflateBinding(layoutInflater)
}

inline fun <reified T : ViewBinding> Fragment.inflate(): Lazy<T> = lazy {
    inflateBinding(layoutInflater)
}

inline fun <reified T : ViewBinding> Dialog.inflate(): Lazy<T> = lazy {
    inflateBinding(layoutInflater)
}

inline fun <reified T : ViewBinding> inflateBinding(layoutInflater: LayoutInflater): T {
    val method = T::class.java.getMethod("inflate", LayoutInflater::class.java)
    return method.invoke(null, layoutInflater) as T
}

inline fun <reified T : ViewBinding> newBindingViewHolder(
    viewGroup: ViewGroup,
    attachToParent: Boolean = false
): T {
    val method = T::class.java.getMethod(
        "inflate",
        LayoutInflater::class.java,
        ViewGroup::class.java,
        Boolean::class.java
    )
    return method.invoke(null, LayoutInflater.from(viewGroup.context), viewGroup, attachToParent) as T
}
