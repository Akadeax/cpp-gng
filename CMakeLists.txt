cmake_minimum_required(VERSION 3.7)

project(gng)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Compile main enginelib library
add_library(enginelib STATIC
    Engine/Animation.h Engine/Animation.cpp
    Engine/AnimationFrame.h Engine/AnimationFrame.cpp
    Engine/AnimatorAnimationEndTransition.h Engine/AnimatorAnimationEndTransition.cpp
    Engine/AnimatorRenderer.h Engine/AnimatorRenderer.cpp
    Engine/AnimatorState.h Engine/AnimatorState.cpp
    Engine/AnimatorTransition.h Engine/AnimatorTransition.cpp
    Engine/base.h Engine/base.cpp
    Engine/Camera.h Engine/Camera.cpp
    Engine/Collider.h Engine/Collider.cpp
    Engine/collisions.h Engine/collisions.cpp
    Engine/Component.h Engine/Component.cpp
    Engine/ConditionalAnimatorTransition.h Engine/ConditionalAnimatorTransition.cpp
    Engine/Entity.h Engine/Entity.cpp
    Engine/EntityKeeper.h Engine/EntityKeeper.cpp
    Engine/Game.h Engine/Game.cpp
    Engine/InputHandler.h Engine/InputHandler.cpp
    Engine/Matrix2x3.h Engine/Matrix2x3.cpp
    Engine/PartialRenderer.h Engine/PartialRenderer.cpp
    Engine/PhysicsBody.h Engine/PhysicsBody.cpp
    Engine/PhysicsHandler.h Engine/PhysicsHandler.cpp
    Engine/Renderer.h Engine/Renderer.cpp
    Engine/Scene.cpp Engine/Scene.h
    Engine/SoundEffect.h Engine/SoundEffect.cpp
    Engine/SoundStream.h Engine/SoundStream.cpp
    Engine/structs.h Engine/structs.cpp
    Engine/SVGParser.h Engine/SVGParser.cpp
    Engine/TextureCache.h Engine/TextureCache.cpp
    Engine/Texture.h Engine/Texture.cpp
    Engine/Transform.h Engine/Transform.cpp
    Engine/utils.h Engine/utils.cpp
    Engine/Vector2f.h Engine/Vector2f.cpp

    # Game files
    Game/Zombie.h Game/Zombie.cpp
    Game/ProjectilePool.h Game/ProjectilePool.cpp
    Game/Projectile.h Game/Projectile.cpp
    Game/PlayerController.h Game/PlayerController.cpp
    Game/PlayerCollider.h Game/PlayerCollider.cpp
    Game/PlayerCamera.h Game/PlayerCamera.cpp
    Game/pch.h Game/pch.cpp
    Game/LevelScene.h Game/LevelScene.cpp
    Game/LadderCollider.h Game/LadderCollider.cpp
    Game/GhostsAndGoblins.h Game/GhostsAndGoblins.cpp
    Game/constants.h
)
target_include_directories(enginelib PUBLIC "Engine")


add_executable(gng Game/main.cpp)
target_link_libraries(gng 
    enginelib
    ${SDL2_LIBRARIES}
    ${SDL2mixer_INCLUDE_DIRS}
    ${SDL2ttf_INCLUDE_DIRS}
    ${SDL2image_INCLUDE_DIRS}
    ${OPENGL_LIBRARIES}
    "-lSDL2_image"
    "-lSDL2_mixer"
    "-lSDL2_ttf"
)
