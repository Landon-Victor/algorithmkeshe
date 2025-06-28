#include "object.h"
#include "game_scene.h"
#include "scene_manager.h"
#include "vector2.h"

// Object
Object::Object() {
    collision_box = CollisionManager::instance()->create_collision_box();
}
Object::Object(GameScene* scene) {
    game_scene = scene;
    collision_box = CollisionManager::instance()->create_collision_box();
}
Object::~Object() {
    if (collision_box != nullptr) {
        CollisionManager::instance()->destroy_collision_box(collision_box);
        collision_box = nullptr;
    }
}

// Wall
Wall::Wall(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(true);
    collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
    collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    collision_box->set_layer_dst(CollisionLayer::Role);
    collision_box->set_layer_src(CollisionLayer::None);
    int a = rand() % 6 + 1;
    if (a > 3) a = 4;
    std::string image_name = "wall" + std::to_string(a);
    image.set_image(image_name);
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
}
Wall::~Wall() {}
void Wall::on_render(const Camera& camera) {
    image.on_render(camera);
}

// Money
Money::Money(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(true);
    collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
    collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    collision_box->set_layer_dst(CollisionLayer::None);
    collision_box->set_layer_src(CollisionLayer::OOO);
    collision_box->set_on_collide([&]() {
        collision_box->set_enabled(false);
        image.set_image("path");
        game_scene->map[logic_pos.y][logic_pos.x] = Content::none;
        });
    image.set_image("money");
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
}
void Money::on_render(const Camera& camera) {
    image.on_render(camera);
}

// Start
Start::Start(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(true);
    collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
    collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    collision_box->set_layer_dst(CollisionLayer::Role);
    collision_box->set_layer_src(CollisionLayer::None);
    image.set_image("start");
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
}
void Start::on_render(const Camera& camera) {
    image.on_render(camera);
}

// Exit
Exit::Exit(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(true);
    collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
    collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    collision_box->set_layer_dst(CollisionLayer::Role);
    collision_box->set_layer_src(CollisionLayer::None);
    image.set_image("exit");
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
}
void Exit::on_render(const Camera& camera) {
    image.on_render(camera);
}

// Path
Path::Path(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(false);
    image.set_image("path");
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
}
void Path::on_render(const Camera& camera) {
    image.on_render(camera);
}

// Locker
Locker::Locker(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(true);
    collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
    collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    collision_box->set_layer_dst(CollisionLayer::None);
    collision_box->set_layer_src(CollisionLayer::OOO);
    collision_box->set_on_collide([&]() {
        collision_box->set_enabled(false);
        image.set_image("path");
        game_scene->map[logic_pos.y][logic_pos.x] = Content::none;
        });
    image.set_image("locker");
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
}
void Locker::on_render(const Camera& camera) {
    image.on_render(camera);
}

// Boss
Boss::Boss(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(true);
    collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
    collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    collision_box->set_layer_dst(CollisionLayer::None);
    collision_box->set_layer_src(CollisionLayer::OOO);
    collision_box->set_on_collide([&]() {
        collision_box->set_enabled(false);
        game_scene->map[logic_pos.y][logic_pos.x] = Content::none;
        image.set_image("path");
        SceneManager::instance()->switch_to(SceneManager::SceneType::Boss);
        });
    image.set_image("path");
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    boss_idle_left.add_frame(ResourcesManager::instance()->find_image("boss_idle_left"), 6);
    boss_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    boss_idle_left.set_interval(150.0f);
    boss_idle_left.set_loop(true);
    boss_idle_left.set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE));
}
void Boss::on_render(const Camera& camera) {
    image.on_render(camera);
    boss_idle_left.on_render(camera);
}

// Trap
Trap::Trap(int x, int y, GameScene* scene) : Object(scene) {
    logic_pos = Vector2(y, x);
    real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
    collision_box->set_enabled(true);
    collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
    collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
    collision_box->set_layer_dst(CollisionLayer::None);
    collision_box->set_layer_src(CollisionLayer::OOO);
    collision_box->set_on_collide([&]() {
        collision_box->set_enabled(false);
        image.set_image("path");
        game_scene->map[logic_pos.y][logic_pos.x] = Content::none;
        });
    image.set_image("trap");
    image.set_position(real_pos);
    image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
}
void Trap::on_render(const Camera& camera) {
    image.on_render(camera);
}

// Player
Player::Player() {
    collision_box = CollisionManager::instance()->create_collision_box();
    collision_box->set_enabled(true);
    collision_box->set_size(Vector2(OBJECT_SIZE - 30, OBJECT_SIZE - 30));
    collision_box->set_layer_dst(CollisionLayer::OOO);
    collision_box->set_layer_src(CollisionLayer::Role);
    collision_box->set_on_collide([&]() {
        real_pos = old_real_pos;
        collision_box->set_position(Vector2(real_pos.x, real_pos.y - OBJECT_SIZE / 2));
        });

    player_idle_left.add_frame(ResourcesManager::instance()->find_image("player_idle_left"), 5);
    player_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    player_idle_left.set_interval(150.0f);
    player_idle_left.set_loop(true);
    player_idle_right.add_frame(ResourcesManager::instance()->find_image("player_idle_right"), 5);
    player_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    player_idle_right.set_interval(150.0f);
    player_idle_right.set_loop(true);
    player_run_left.add_frame(ResourcesManager::instance()->find_image("player_run_left"), 10);
    player_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    player_run_left.set_interval(0.1f);
    player_run_left.set_loop(true);
    player_run_right.add_frame(ResourcesManager::instance()->find_image("player_run_right"), 10);
    player_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    player_run_right.set_interval(0.1f);
    player_run_right.set_loop(true);
    current_animation = &player_idle_right;
}
Player::~Player() {
    if (collision_box != nullptr) {
        CollisionManager::instance()->destroy_collision_box(collision_box);
        collision_box = nullptr;
    }
}
const Vector2& Player::get_position() {
    return real_pos;
}
void Player::set_position(const Vector2& pos, GameScene* g) {
    logic_pos = Vector2(pos.y, pos.x);
    real_pos = Vector2((pos.y + 0.5f) * OBJECT_SIZE, pos.x * OBJECT_SIZE + OBJECT_SIZE);
    old_real_pos = real_pos;
    collision_box->set_position(Vector2(real_pos.x, real_pos.y - OBJECT_SIZE / 2));
    game_scene = g;
}
void Player::on_input(const ExMessage& msg) {
    switch (msg.message) {
    case WM_KEYDOWN:
        switch (msg.vkcode) {
        case 0x41: is_left_key_down = true; break;
        case 0x44: is_right_key_down = true; break;
        case 0x57: is_up_key_down = true; break;
        case 0x53: is_down_key_down = true; break;
        }
        break;
    case WM_KEYUP:
        switch (msg.vkcode) {
        case 0x41: is_left_key_down = false; break;
        case 0x44: is_right_key_down = false; break;
        case 0x57: is_up_key_down = false; break;
        case 0x53: is_down_key_down = false; break;
        }
        break;
    default:
        break;
    }
}
void Player::on_update(int delta) {
    old_real_pos = real_pos;
    int xdirection = is_right_key_down - is_left_key_down;
    if (xdirection != 0) {
        is_facing_right = xdirection > 0;
        current_animation = is_facing_right ? &player_run_right : &player_run_left;
        real_pos.x += xdirection * 0.3f * delta;
    }
    int ydirection = is_down_key_down - is_up_key_down;
    if (ydirection != 0) {
        real_pos.y += ydirection * 0.3f * delta;
        current_animation = is_facing_right ? &player_run_right : &player_run_left;
    }
    if (xdirection == 0 && ydirection == 0) {
        current_animation = is_facing_right ? &player_idle_right : &player_idle_left;
    }
    current_animation->on_update(delta);
    collision_box->set_position(Vector2(real_pos.x, real_pos.y - OBJECT_SIZE / 2));
    CollisionManager::instance()->process_collide();
}
void Player::on_render(const Camera& camera) {
    if (current_animation != nullptr) {
        current_animation->set_position(real_pos);
        current_animation->on_render(camera);
    }
}