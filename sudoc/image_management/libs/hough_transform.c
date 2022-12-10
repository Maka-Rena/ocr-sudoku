#include "../include/hough_transform.h"

const double PI = 3.141592653589793;

/// @brief Free an image and set the pointer to NULL. It has double free protection.
/// @param image The image to free.
void CV_FREE(Image **image)
{
    if (*image != NULL)
    {
        FREE((*image)->data);
        FREE(*image);
    }
}

Image *CV_INIT(int channels, int height, int width)
{
    Image *image = malloc(sizeof(Image));

    image->c = channels;
    image->w = width;
    image->h = height;

    image->data = malloc(channels * height * width * sizeof(pixel_t));
    if (image->data == NULL)
        errx(1, "Out of memory");
    return image;
}

Image *CV_COPY(const Image *src)
{

    Image *copy = CV_INIT(src->c, src->h, src->w);

    memcpy(copy->data, src->data, src->c * src->h * src->w * sizeof(pixel_t));

    return copy;
}

/// @brief Convert an SDL_Surface to an Image.
/// @param surface The SDL_Surface to convert.
/// @return The new image.
Image *CV_SURFACE_TO_IMG(SDL_Surface *surface)
{
    SDL_LockSurface(surface);

    int w = surface->w;
    int h = surface->h;

    SDL_PixelFormat *format = surface->format;
    Uint32 *pixels = surface->pixels;
    Image *image = CV_INIT(3, h, w);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            Uint32 pixel = pixels[i * w + j];
            Uint8 r, g, b;

            SDL_GetRGB(pixel, format, &r, &g, &b);

            PIXEL(image, 0, i, j) = norm(r / 255.0);
            PIXEL(image, 1, i, j) = norm(g / 255.0);
            PIXEL(image, 2, i, j) = norm(b / 255.0);
        }
    }

    SDL_UnlockSurface(surface);
    return image;
}

/// @brief Convert an Image to an SDL_Surface.
/// @param image The image to convert.
/// @return The new SDL_Surface.
SDL_Surface *CV_IMG_TO_SURFACE(const Image *image)
{

    SDL_Surface *surface = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);

    SDL_LockSurface(surface);

    SDL_PixelFormat *format = surface->format;
    Uint32 *pixels = surface->pixels;

    if (image->c == 3)
    {
        for (int i = 0; i < image->h; i++)
        {
            for (int j = 0; j < image->w; j++)
            {
                Uint8 r = norm(PIXEL(image, 0, i, j)) * 255;
                Uint8 g = norm(PIXEL(image, 1, i, j)) * 255;
                Uint8 b = norm(PIXEL(image, 2, i, j)) * 255;

                Uint32 pixel = SDL_MapRGB(format, r, g, b);
                pixels[i * image->w + j] = pixel;
            }
        }
    }
    else if (image->c == 1)
    {
        for (int i = 0; i < image->h; i++)
        {
            for (int j = 0; j < image->w; j++)
            {
                Uint8 p = norm(PIXEL(image, 0, i, j)) * 255;
                Uint32 pixel = SDL_MapRGB(format, p, p, p);
                pixels[i * image->w + j] = pixel;
            }
        }
    }

    SDL_UnlockSurface(surface);
    return surface;
}


/// @brief Find Lines in an image using the Hough Transform algorithm.
/// @param src The source image
/// @param threshold The threshold value, representing the minimum number of intersections to detect a line
/// @param nlines The number of lines that will be returned.
/// @return An array of lines where 2n is rho and 2n+1 is theta.
int *Hough_lines(const Image *src, int threshold, int *nlines)
{
    *nlines = 0;

    int w = src->w;
    int h = src->h;

    int *accumulator = (int *)calloc(w * h * 180, sizeof(int));
    if (accumulator == NULL)
        errx(1, "Out of memory");

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (PIXEL(src, 0, y, x) == 0)
                continue;

            for (int t = 0; t < 180; t++)
            {
                double theta = (double)t * PI / 180;            // theta in radian
                double rho = (x * cos(theta) + y * sin(theta)); // rho in pixel

                int r = (int)ceil(rho);
                // smart workaround to avoid negative values
                // since the accumulator is a 1D array
                // we need to shift the values to the right
                // or only
                accumulator[t * w * h + r + w * h / 2]++;
            }
        }
    }
    int *lines = (int *)malloc(sizeof(int) * w * h * 180 * 2);
    if (lines == NULL)
        errx(1, "Out of memory");

    // we fill the lines array
    int n = 0;
    for (int i = 0; i < w * h * 180; i++)
    {
        if (accumulator[i] < threshold)
            continue;

        lines[n * 2] = i % (w * h) - w * h / 2; // rho
        lines[n * 2 + 1] = i / (w * h);         // theta
        n++;
    }

    if (n == 0)
    {
        FREE(accumulator);
        FREE(lines);
        return NULL;
    }

    int *rlines = (int *)realloc(lines, sizeof(int) * n * 2);
    if (rlines == NULL)
    {
        FREE(accumulator);
        FREE(lines);
        return NULL;
    }
    else
        lines = rlines;

    // sort the lines by rho
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (lines[i * 2] > lines[j * 2])
            {
                int tmp = lines[i * 2];
                lines[i * 2] = lines[j * 2];
                lines[j * 2] = tmp;

                tmp = lines[i * 2 + 1];
                lines[i * 2 + 1] = lines[j * 2 + 1];
                lines[j * 2 + 1] = tmp;
            }
        }
    }

    // cleanup and return
    *nlines = n;
    FREE(accumulator);
    return lines;
}

/// @brief Merge lines that are close to each other.
/// @param lines The lines array
/// @param nlines The number of lines
/// @param threshold The threshold value to merge lines
/// @param n The number of merged lines
/// @return An array of simplified lines where 2n is rho and 2n+1 is theta.
int *Hough_merge_lines(int *lines, int nlines, int threshold, int *n)
{
    *n = 0;

    if (lines == NULL)
        return NULL;

    int *merged_lines = (int *)malloc(sizeof(int) * nlines * 2);
    memset(merged_lines, 0, sizeof(int) * nlines * 2);

    int j = 0;
    for (int i = 0; i < nlines * 2; i += 2)
    {
        int rho = lines[i];
        int theta = lines[i + 1];

        int found = 0;
        for (int k = 0; k < j; k += 2)
        {
            if (abs(rho - merged_lines[k]) < threshold && abs(theta - merged_lines[k + 1]) < threshold)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            merged_lines[j] = rho;
            merged_lines[j + 1] = theta;
            j += 2;
        }
    }

    *n = j / 2;
    return merged_lines;
}

/// @brief Build a uint32 from r, g, b values. (0 <= r, g, b <= 255) and a is skipped
/// @param r The red value
/// @param g The green value
/// @param b The blue value
/// @return The uint32 value
Uint32 CV_RGB(Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 color = 0;
    return color | r << 16 | g << 8 | b;
}

Image *Hough_draw_point(const Image *src, Image *dst, int x, int y, int width, Uint32 color)
{
    if (src == NULL)
        errx(1, "Hough_draw_point: src is NULL");

    if (x < 0 || x >= dst->w || y < 0 || y >= dst->h)
        return dst;

    for (int c = 0; c < dst->c; c++)
    {
        for (int i = -width / 2; i <= width / 2; i++)
        {
            for (int j = -width / 2; j <= width / 2; j++)
            {
                if (x + i < 0 || x + i >= dst->w || y + j < 0 || y + j >= dst->h)
                    continue;

                int r = (color >> (16 - c * 8)) & 0xff;
                PIXEL(dst, c, y + j, x + i) = r / 255.0;
            }
        }
    }

    return dst;
}

Image *Hough_draw_line(const Image *src, Image *dst, int x1, int y1, int x2, int y2, int width, Uint32 color)
{

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < width; j++)
                Hough_draw_point(src, dst, x1 + i, y1 + j, width, color);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err = err - dy;
            x1 = x1 + sx;
        }
        if (e2 < dx)
        {
            err = err + dx;
            y1 = y1 + sy;
        }
    }

    return dst;
}

/// @brief Draw lines on an image.
/// @param src The source image
/// @param dst The destination image
/// @param lines The lines array
/// @param nlines The number of lines
/// @param weight The weight of the lines
/// @param color The color of the lines
/// @return The destination image
Image *Hough_draw_lines(const Image *src, Image *dst, int *lines, int nlines, int weight, Uint32 color)
{

    for (int i = 0; i < nlines; i++)
    {
        int rho = lines[i * 2];
        int theta = lines[i * 2 + 1];

        if (theta == 0)
            Hough_draw_line(src, dst, rho, 0, rho, dst->h, weight, color);
        else
        {
            // y = mx + p
            // m = tan(theta)
            // p = rho / sin(theta)
            // 2000 is an arbitrary value and should be enough to draw the line
            double a = cos(theta * PI / 180.0);
            double b = sin(theta * PI / 180.0);
            double x0 = a * rho;
            double y0 = b * rho;
            double x1 = x0 + 2000 * (-b);
            double y1 = y0 + 2000 * a;
            double x2 = x0 - 2000 * (-b);
            double y2 = y0 - 2000 * a;

            Hough_draw_line(src, dst, x1, y1, x2, y2, weight, color);
        }
    }

    return dst;
}

/// @brief Draw a circle on an image
/// @param src The source image
/// @param dst The destination image
/// @param x The x coordinate of the center
/// @param y The y coordinate of the center
/// @param r The radius of the circle
/// @param width The width of the circle
/// @param color The color of the circle
/// @return
Image *Hough_draw_circle(const Image *src, Image *dst, int x, int y, int r, int width, Uint32 color)
{
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int _x = 0;
    int _y = r;

    Hough_draw_point(src, dst, x, y + r, width, color);
    Hough_draw_point(src, dst, x, y - r, width, color);
    Hough_draw_point(src, dst, x + r, y, width, color);
    Hough_draw_point(src, dst, x - r, y, width, color);

    while (_x < _y)
    {
        if (f >= 0)
        {
            _y--;
            ddF_y += 2;
            f += ddF_y;
        }
        _x++;
        ddF_x += 2;
        f += ddF_x;

        Hough_draw_point(src, dst, x - _x, y + _y, width, color);
        Hough_draw_point(src, dst, x + _x, y + _y, width, color);
        Hough_draw_point(src, dst, x + _x, y - _y, width, color);
        Hough_draw_point(src, dst, x - _x, y - _y, width, color);
        Hough_draw_point(src, dst, x + _y, y + _x, width, color);
        Hough_draw_point(src, dst, x - _y, y + _x, width, color);
        Hough_draw_point(src, dst, x + _y, y - _x, width, color);
        Hough_draw_point(src, dst, x - _y, y - _x, width, color);
    }

    return dst;
}

/// @brief Find the orientation of an image using lines.
/// @param lines The lines array
/// @param nlines The number of lines
/// @return The orientation of the image in degrees.
float Find_orientation(int *lines, int nlines)
{
    int *histo = (int *)malloc(sizeof(int) * 180);
    memset(histo, 0, sizeof(int) * 180);

    for (int i = 0; i < nlines; i++)
    {
        int theta = lines[i * 2 + 1];
        histo[theta]++;
    }

    int max = 0;
    int max_theta = 0;
    for (int i = 0; i < 180; i++)
    {
        if (histo[i] > max)
        {
            max = histo[i];
            max_theta = i;
        }
    }

    FREE(histo);
    return max_theta;
}

/// @brief Find all intersections between two lines
/// @param lines Lines to find intersections
/// @param nlines Number of lines
/// @param nintersection Number of intersections
/// @return Array of intersections where 2n and 2n+1 are x and y coordinates
int *Compute_intersections(int *lines, int nlines, int *nintersection, int width, int height)
{
    int *intersection = calloc(nlines * nlines * 2,sizeof(int));

    int j = 0;
    for (int i = 0; i < nlines; i++)
    {
        int rho1 = lines[i * 2];
        int theta1 = lines[i * 2 + 1];

        for (int k = i + 1; k < nlines; k++)
        {
            int rho2 = lines[k * 2];
            int theta2 = lines[k * 2 + 1];

            float a = cos(theta1 * PI / 180.0);
            float b = sin(theta1 * PI / 180.0);
            float c = cos(theta2 * PI / 180.0);
            float d = sin(theta2 * PI / 180.0);

            float det = a * d - b * c;
            if (det == 0)
                continue;

            float x = (d * rho1 - b * rho2) / det;
            float y = (-c * rho1 + a * rho2) / det;
            if (x <= 0 || y <= 0 || x >= width || y >= height)
                continue;
            intersection[j * 2] = (int)x;
            intersection[j * 2 + 1] = (int)y;
            j++;
        }
    }
    intersection = realloc(intersection, sizeof(int) * j * 2);
    *nintersection = j;

    return intersection;
}

/// @brief Find all boxes in a grid
/// @param intersections Intersections to find boxes
/// @param nintersections Number of intersections
/// @param nboxes Number of boxes
/// @return Array of boxes where 4n, 4n+1, 4n+2 and 4n+3 are x and y coordinates of the 4 corners
int *Get_grid(int *intersections, int nintersections)
{
    int *res = calloc(4,sizeof(int));
    int minx = intersections[0];
    int maxx = 0;
    int miny = intersections[1];
    int maxy = 0;
    for (int i = 0; i < nintersections; i++)
    {
        int x = intersections[i * 2];
        int y = intersections[i * 2 + 1];

        if (x < minx)
            minx = x;
        if (x > maxx)
            maxx = x;
        if (y < miny)
            miny = y;
        if (y > maxy)
            maxy = y;
    }
    res[0] = minx;
    res[1] = miny;
    res[2] = maxx;
    res[3] = maxy;
    return res;
}


