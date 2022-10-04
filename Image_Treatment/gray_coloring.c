#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void Draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
//"SDL_Texture* colored" must be added as a paramter if we uncomment parts of the code 
void Event_loop(SDL_Renderer* renderer, SDL_Texture* grayscale) 
{
    SDL_Event event;
    //SDL_Texture* t = colored;
    SDL_Texture* t = grayscale;
    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    Draw(renderer, t);
                }
                break;
            /*case SDL_KEYDOWN :
                if (t == colored)
                {
                    draw(renderer, grayscale);
                    t = grayscale;
                }
                else
                    {
                        draw(renderer, colored);
                        t = colored;
                    }
                break;*/
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* Load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* res = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);
    return res;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 __Pixel_To_Grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    
    uint average = 0.3*r + 0.59*g +0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void Surface_To_Grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface);
    int count = 0;
    while (count < len)
    {
        pixels[count] = __Pixel_To_Grayscale(pixels[count], format);
        count++;
    }
    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    // TODO:
    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Plain Window", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* surface = Load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w, surface->h);
    
    // - Create a texture from the colored surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface into grayscale.
    Surface_To_Grayscale(surface);

    // - Create a new texture from the grayscale surface.
    SDL_Texture* texture_gray = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_gray == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //to save the grayscale image in a png file
    IMG_SavePNG(surface, "final_picture.png");

    // - Free the surface.
    SDL_FreeSurface(surface);
    
    // - Dispatch the events.
    //event_loop(renderer, texture, texture_gray);
    Event_loop(renderer, texture_gray);

    // - Destroy the objects.

    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_gray);
    SDL_Quit();

    return EXIT_SUCCESS;
}