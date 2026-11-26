plugins {
    id("java-library")
}
java {
    val javaVersion = libs.versions.javaVersion.get()
    sourceCompatibility = JavaVersion.toVersion(javaVersion.toString())
    targetCompatibility = JavaVersion.toVersion(javaVersion.toString())
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))))
    annotationProcessor("com.google.auto.service:auto-service:1.1.1")
    compileOnly("com.google.auto.service:auto-service:1.1.1")
    implementation("com.squareup:javapoet:1.13.0")
    implementation(project(":black-reflection"))
}
