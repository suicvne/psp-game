#ifndef ___CAMERA_H___
#define ___CAMERA_H___



extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int HALF_SCREEN_WIDTH;
extern const int HALF_SCREEN_HEIGHT;

struct _vector_t;


typedef struct _camera_t {

  float x, y; //the X and Y offsets at which to draw everything else.

} camera_t;

struct _camera_t* camera_create(int x, int y);
void camera_destroy(struct _camera_t* camera);

struct _vector_t camera_player_to_world(const struct _camera_t* camera);

void camera_move(struct _camera_t* camera, const struct _vector_t movement_amount);

int camera_get_player_origin(int* x, int* y);

#endif //___CAMERA_H___
