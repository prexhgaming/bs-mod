plugins {
    id("java-library")
}
java {
    val javaVersion = libs.versions.javaVersion.get()
    sourceCompatibility = JavaVersion.toVersion(javaVersion.toString())
    targetCompatibility = JavaVersion.toVersion(javaVersion.toString())
}
