#include "main.h"

extern struct triangle **triangles;
extern struct object *objects;
extern int nt;
extern struct camera *cam;
uint32_t *memory;
extern float *zbuffer;


LRESULT CALLBACK 
window_proc(HWND window, 
            UINT message, 
            WPARAM w_param, 
            LPARAM l_param)
{
    switch(message)
    {
        case WM_KEYDOWN:
        {
            switch(w_param)
            {
				case 'N':
				{	
					objects->sun->x += 25;
					break;
				}
				case 'I':
				{	
					objects->rx += 25;
					break;
				}
				case 'K':
				{	
					objects->rx -= 25;
					break;
				}
				case 'U':
				{	
					objects->ry += 25;
					break;
				}
				case 'O':
				{	
					objects->ry -= 25;
					break;
				}
				case 'J':
				{	
					objects->z += 25;
					break;
				}
				case 'L':
				{	
					objects->z -= 25;
					break;
				}
                case 'Z':
                {
                    cam->z += 25 * cos(cam->ry);
					cam->x += 25 * sin(cam->ry);
					break;
				};
				case 'S':
                {
                    cam->z -= 25 * cos(cam->ry);
					cam->x -= 25 * sin(cam->ry);
					break;
				};
				case 'Q':
                {
                    cam->z += 25 * sin(cam->ry);
					cam->x -= 25 * cos(cam->ry);
					break;
				};
				case 'D':
                {	
                    cam->z -= 25 * sin(cam->ry);
					cam->x += 25 * cos(cam->ry);
					break;
				};
				case VK_LEFT:
				{
					cam->ry -= 0.05;
					cam->cry = cos(-cam->ry);
					cam->sry = sin(-cam->ry);
					break;
				};
				case VK_RIGHT:
				{
					cam->ry += 0.05;
					cam->cry = cos(-cam->ry);
					cam->sry = sin(-cam->ry);
					break;
				};
				case VK_UP:
				{
					cam->rx -= 0.05;
					cam->crx = cos(-cam->rx);
					cam->srx = sin(-cam->rx);
					break;
				};
				case VK_DOWN:
				{
					cam->rx += 0.05;
					cam->crx = cos(-cam->rx);
					cam->srx = sin(-cam->rx);
					break;
				};
				case VK_SPACE:
				{
					cam->y += 25;
					break;
				};
				case VK_SHIFT:
				{
					cam->y -= 25;
					break;
				};
            }
        } break;
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;
        
        default:
        {
            return DefWindowProc(window, 
                                 message, 
                                 w_param, 
                                 l_param);
        }
    }
    
    return 0;
}

void init_object(void)
{
	FILE *f = fopen("test.cu", "r");
	if (f == NULL)
		printf ("error ");
	fseek(f, 0, SEEK_END);
	size_t size_string = ftell(f);
	fseek(f, 0, SEEK_SET);  
	char *string = calloc(size_string, 1);
	fread(string, size_string, 1, f);
	fclose(f);
	struct object *obj = create_object(NULL, -100,0,200,0,3.14,0);
	struct object *obj2 = create_object(NULL, 100,0,200,0,3.14,0);
	struct object *obj3 = create_object(NULL, -300,0,200,0,3.14,0);
	struct object *obj4 = create_object(NULL, 300,0,200,0,3.14,0);
	cuboid_to_obj(obj, string);
	cuboid_to_obj(obj2, string);
	cuboid_to_obj(obj3, string);
	cuboid_to_obj(obj4, string);
	free(string);
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
    // window creation
    WNDCLASS window_class = {};
    const wchar_t class_name[] = L"MyWindowClass";
    
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = instance;
    window_class.lpszClassName = class_name;
    window_class.hCursor = LoadCursor(0, IDC_CROSS);
    if(!RegisterClass(&window_class))
    {
        MessageBox(0, L"RegisterClass failed", 0, 0);
        return GetLastError();
    }
    
    HWND window = CreateWindowEx(0,
                                 class_name,
                                 L"Window",
                                 WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 client_width + 17,
                                 client_height + 39,
                                 0,
                                 0,
                                 instance,
                                 0);
    
    if(!window)
    {
        MessageBox(0, L"CreateWindowEx failed", 0, 0);
        return GetLastError();
    }
    
    // allocate memory	
    memory = (uint32_t*)malloc(sizeof(uint32_t) * client_width * client_height * 2);
	zbuffer = malloc(sizeof(float) * client_width * client_height);
    
    // create BITMAPINFO struct for StretchDIBits
    
    BITMAPINFO bitmap_info;
    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = client_width;
    bitmap_info.bmiHeader.biHeight = client_height;
    bitmap_info.	bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    HDC hdc = GetDC(window);
    uint8_t running = 1;
    int a = GetTickCount();
	cam = init_cam();
	float **land = generate_land(10,10,20,50);
	//init_object();
    for (int i = 0;;i++)
    {
		clear_screen(0x000000);
		draw_scene(objects, 0, 0, 0, 0, 0, 0, 0);
		//transform(objects);
        MSG msg;
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {	
            if(msg.message == WM_QUIT) running = 0;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }    

		StretchDIBits(hdc,
                      0,
                      0,
                      client_width,
                      client_height,
                      0,
                      0,
                      client_width,
                      client_height,
                      memory,
                      &bitmap_info, 	
                      DIB_RGB_COLORS,
                      SRCCOPY
                      );
    }
    
    return 0;
}