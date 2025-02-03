#include <iostream>
#include <cmath>
#include <list>
#include "parser.h"
#include "renderer.h"

#define WIDTH 1500
#define HEIGHT 800
#define COLOR_WHITE 0xffffff
#define COLOR_BLACK 0x000000


int main(){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("GameEngine", WIDTH, HEIGHT, 0);
    if(window == NULL) exit(1);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if(renderer == NULL) exit(1);

    bool is_running = true;
    SDL_Event event;

    // our figure

    mesh figure;

    std::string path;

    std::cout << "Please enter path to the file: "; // for example "/Users/oleg/Documents/GitHub/game_engine/data/mountains.obj"
    std::cin >> path;

    LoadFromObjectFile(path, &figure.tris);

    // projection matrix

    matrix4x4 proj_mat;
    proj_mat = Matrix_MakeProjection(90.0f, (float)HEIGHT / (float)WIDTH, 0.1f, 1000.0f);

    SDL_Rect clear_rect;

    float theta = 0.0f;
    float Y_angle = 0.0f;

    vec3d vector_camera = {0 ,0, 0};
    vec3d look_direction;


    // -------------------- main cycle ----------------------

    while (is_running) {

        vec3d v_forvard = VectorMul(look_direction, 0.5f);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
            if (event.key.scancode == SDL_SCANCODE_E) { // UP
                vector_camera.y += 0.3;
            }
            if (event.key.scancode == SDL_SCANCODE_Q) { // DOWN
                vector_camera.y -= 0.3;
            }
            if (event.key.scancode == SDL_SCANCODE_D) { // RIGHT TURN
                Y_angle += 0.05;
            }
            if (event.key.scancode == SDL_SCANCODE_A) { // LEFT TURN
                Y_angle -= 0.05;
            }
            if (event.key.scancode == SDL_SCANCODE_W) { // FORWARD
                vector_camera = VectorAdd(vector_camera, v_forvard);
            }
            if (event.key.scancode == SDL_SCANCODE_S) { // BACKWARD
                vector_camera = VectorSub(vector_camera, v_forvard);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear( renderer );

        matrix4x4 translation_mat;
        translation_mat = Matrix_MakeTranslation( 0.0f, 0.0f, 5.0f ); 

        matrix4x4 world_mat;
        world_mat = Matrix_MakeIdentity();
        world_mat = Matrix_MultiplyMatrix(world_mat, translation_mat);

        vec3d vector_up = { 0, -1, 0 };
        vec3d vector_target = { 0, 0, 1 };
        matrix4x4 camera_rotation_mat = Matrix_MakeRotationY(Y_angle);
        look_direction = MatrixMultiplyVector(camera_rotation_mat, vector_target);
        vector_target = VectorAdd(vector_camera, look_direction);

        matrix4x4 camera_mat = Matrix_PointAt(vector_camera, vector_target, vector_up);

        matrix4x4 view_mat = Matrix_QuickInverse(camera_mat);
        
        std::vector<triangle> triangles_to_render;

        for(auto tris: figure.tris){ // draw triangles
            triangle tri_projected, tri_transformed, tri_viewed;
            
            tri_transformed.points[0] = MatrixMultiplyVector(world_mat, tris.points[0]);
            tri_transformed.points[1] = MatrixMultiplyVector(world_mat, tris.points[1]);
            tri_transformed.points[2] = MatrixMultiplyVector(world_mat, tris.points[2]);

            vec3d vec1, vec2, normal;

            vec1 = VectorSub(tri_transformed.points[1], tri_transformed.points[0]);
            vec2 = VectorSub(tri_transformed.points[2], tri_transformed.points[0]);


            normal = Vector_CrossProduct(vec1, vec2);
            normal = VectorNormalise(normal);

            vec3d CamRay = VectorSub(tri_transformed.points[0], vector_camera);
            

            if( DotProduct(normal, CamRay) < 0.0f ){

                vec3d light_direction = { 0.0f, 0.0f, -1.0f };
                light_direction = VectorNormalise(light_direction);

                float color = fmax(0.1f, DotProduct(light_direction, normal));
                
                tri_viewed.points[0] = MatrixMultiplyVector(view_mat, tri_transformed.points[0]);
                tri_viewed.points[1] = MatrixMultiplyVector(view_mat, tri_transformed.points[1]);
                tri_viewed.points[2] = MatrixMultiplyVector(view_mat, tri_transformed.points[2]);

                tri_viewed.r = color; tri_viewed.g = color; tri_viewed.b = color;

                int n_clipped_tris = 0;
                triangle clipped[2];
                n_clipped_tris = Triangle_ClipAgainstPlane({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f}, tri_viewed, clipped[0], clipped[1]);

                for(int n = 0; n < n_clipped_tris; n++){
                    tri_projected.points[0] = MatrixMultiplyVector(proj_mat, clipped[n].points[0]);
                    tri_projected.points[1] = MatrixMultiplyVector(proj_mat, clipped[n].points[1]);
                    tri_projected.points[2] = MatrixMultiplyVector(proj_mat, clipped[n].points[2]);

                    tri_projected.r = clipped[n].r; tri_projected.g = clipped[n].g; tri_projected.b = clipped[n].b;


                    tri_projected.points[0] = VectorDiv(tri_projected.points[0], tri_projected.points[0].w);
                    tri_projected.points[1] = VectorDiv(tri_projected.points[1], tri_projected.points[1].w);
                    tri_projected.points[2] = VectorDiv(tri_projected.points[2], tri_projected.points[2].w);

                    // scaling
                    vec3d offset_vec = { 1, 1, 0 };
                    tri_projected.points[0] = VectorAdd(tri_projected.points[0], offset_vec);
                    tri_projected.points[1] = VectorAdd(tri_projected.points[1], offset_vec);
                    tri_projected.points[2] = VectorAdd(tri_projected.points[2], offset_vec);

                    tri_projected.points[0].x *= 0.5f * (float)WIDTH;
                    tri_projected.points[0].y *= 0.5f * (float)HEIGHT;
                    tri_projected.points[1].x *= 0.5f * (float)WIDTH;
                    tri_projected.points[1].y *= 0.5f * (float)HEIGHT;
                    tri_projected.points[2].x *= 0.5f * (float)WIDTH;
                    tri_projected.points[2].y *= 0.5f * (float)HEIGHT;


                    triangles_to_render.push_back(tri_projected);
                }
            }
        }

        std::sort(triangles_to_render.begin(), triangles_to_render.end(),[](triangle &t1, triangle &t2){
            float z1 = ( t1.points[0].z + t1.points[1].z + t1.points[2].z);
            float z2 = ( t2.points[0].z + t2.points[1].z + t2.points[2].z);
            return z1 > z2;
        });

        for(auto &tri_to_roaster: triangles_to_render){

            triangle clipped[2];
            std::list<triangle> list_of_triangles;
            list_of_triangles.push_back(tri_to_roaster);
            int n_new_triangles = 1;

            for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (n_new_triangles > 0)
				{
					triangle test = list_of_triangles.front();
					list_of_triangles.pop_front();
					n_new_triangles--;

					switch (p)
					{
					case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ (float)WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					for (int w = 0; w < nTrisToAdd; w++)
						list_of_triangles.push_back(clipped[w]);
				}
				n_new_triangles = list_of_triangles.size();
			}

            for(auto it: list_of_triangles)
                RenderTriangle(renderer, it.points[0].x, it.points[0].y,
                                     it.points[1].x, it.points[1].y, 
                                     it.points[2].x, it.points[2].y, it.r, it.g, it.b);
        }

        SDL_RenderPresent( renderer );
    }
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit(); 

    return 0;
}