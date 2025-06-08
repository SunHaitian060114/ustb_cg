#ifndef CONFIG_H
#define CONFIG_H
/***————————游戏配置数据————————***/
#define GAME_WIDTH   512
#define GAME_HEIGHT  768
#define GAME_TITLE "飞机大战 v1.0"         //标题
#define GAME_RES_PATH "./plane.rcc"       //rcc
#define GAME_ICON "://resource/pic.png"
#define GAME_RATE 5

/***————————地图配置数据————————***/
#define MAP_PATH "://resource/BackGroundScene.jpg"   //地图路径
#define MAP_SCROLL_SPEED 1               //地图滚动速度


/***————————飞机配置数据————————***/
#define HERO_PLANE "://resource/MyPlane.jpg"

/***————————子弹配置数据————————***/
#define BULLET_PATH "://resource/Bullet_01 (2).jpg"
#define BULLET_SPEED 9
#define BULLET_INTERVAL 50
#define BULLET_NUM 30

/***————————敌机配置数据————————***/
#define ENEMY_PATH "://resource/enemy.png"
#define ENEMY_SPEED 2
#define ENEMY_NUM 20
#define ENEMY_INTERVAL 60

/***————————爆炸配置数据————————***/
#define BOMB_PATH "://resource/bomb-%1.png"
#define BOMB_NUM 20//爆炸数量
#define BOMB_MAX 16//爆炸索引
#define BOMB_INTERVAL 20//爆炸贴图时间间隔

/***————————音效配置数据————————***/
#define SOUND_BACKGROUND ":/resource/BGM_MEGALOVANIA.wav"
#define SOUND_BOMB ":/resource/sound_bomb.wav"

/***————————结束配置数据————————***/
#define GAMEOVER_JPG "://resource/gameover.png"
#define GAMEOVER_RESTART "://resource/restart.png"

/***————————开始配置数据————————***/
#define BUTTON_STRAT "://resource/start.png"
#define BUTTON_QUIT "://resource/quit.png"

/***————————暂停配置数据————————***/
#define PAUSE_IMAGE "://resource/pause.png"

#endif // CONFIG_H
