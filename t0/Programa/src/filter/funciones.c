#include <stdio.h>
#include <stdlib.h>
#include "../imagelib/imagelib.h"


int is_white(Color color){
    if (color.R==255 && color.G==255 && color.B == 255)
    {
        return 1;
    }
    return 0;
}

int is_corner(int row, int col, int width, int height){
    if (row==0 && (col == 0 || col == width-1))
    {
        return 1;
    }
    else if (row == height - 1 && (col == 0 || col == width - 1))
    {
        return 1;
    } else{
        return 0;
    }
}

int is_up(int row, int col, int width, int height){
    if (row == 0 && (0<col && col<width-1))
    {
        return 1;
    }
    return 0;
}

int is_down(int row, int col, int width, int height){
    if (row == height-1 && (0<col && col<width-1))
    {
        return 1;
    }
    return 0;
}

int is_left(int row, int col, int width, int height){
    if (col == 0 && (0<row && row<height-1))
    {
        return 1;
    }
    return 0;
}

int is_right(int row, int col, int width, int height){
    if (col == width-1 && (0<row && row<height-1))
    {
        return 1;
    }
    return 0;
}

void for_normal(Image *img, int *red, int *green, int *blue, int col, int row){
    int cont = 0;
    // printf("PIXEL[%d][%d]\n", row, col);
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = col-1; j <= col+1; j++)
        {
            red[cont] = img->pixels[i][j].R;
            green[cont] = img->pixels[i][j].G;
            blue[cont] = img->pixels[i][j].B;
            cont++;
        }
    }
}

void for_borde(Image *img, int *red, int *green, int *blue, int col, int row, int lugar){
    int cont = 0, in_row, term_row, in_col, term_col, condition=0;
    int repetido = 1;
    if (lugar <= 2)
    {
        if (lugar == 1)
        {
            in_row = row;
            term_row = row + 1;
        } else{
            in_row = row-1;
            term_row = row;
        }
        
        in_col = col - 1;
        term_col = col + 1;
        condition = row;
    } else if (lugar == 5){
        if (col == 0 && row == 0)
        {
            in_row = row;
            term_row = row + 1;
            in_col = col;
            term_col = col + 1;
        }
        else if (row == 0 && col == img->width-1){
            in_row = row;
            term_row = row + 1;
            in_col = col - 1;
            term_col = col;
        }
        else if (row == img->height - 1 && col == 0)
        {
            in_row = row-1;
            term_row = row;
            in_col = col;
            term_col = col+1;
        }
        else
        {
            in_row = row-1;
            term_row = row;
            in_col = col - 1;
            term_col = col;
        }
    }
    else
    {
        // acá están al revés para dar vuelta el for
        if (lugar == 3)
        {
            in_row = col;
            term_row = col + 1;
        } else{
            in_row = col - 1;
            term_row = col;
        }
        
        in_col = row - 1;
        term_col = row + 1;
        condition = col;
    }
    int rial_row = 0, rial_col = 0;
    // printf("in row: %d term row:%d in_col: %d term_col: %d row: %d col: %d condicion: %d\n", in_row, term_row, in_col, term_col, row, col, condition);
    for (int i = in_row; i <= term_row; i++)
    {
        for (int j = in_col; j <= term_col; j++)
        {
            if (lugar<=2 || lugar==5)
            {
                rial_row = i;
                rial_col = j;
            } else{
                rial_row = j;
                rial_col = i;
            }

            red[cont] = img->pixels[rial_row][rial_col].R;
            green[cont] = img->pixels[rial_row][rial_col].G;
            blue[cont] = img->pixels[rial_row][rial_col].B;
            cont++;
            //printf("PIXEL[%d][%d]\n", row, col);

           if (repetido < 2 && i == condition && lugar!=5)
            {
                // repetirá la fila ó columna con los valores en el borde
                // ya que si esta arriba o abajo o en uno de los lados se da vuelta el for 
                // y la condición cambia
                red[cont] = img->pixels[rial_row][rial_col].R;
                green[cont] = img->pixels[rial_row][rial_col].G;
                blue[cont] = img->pixels[rial_row][rial_col].B;
                cont++;
                repetido++;
            }

            if (lugar ==5 && ((rial_row == 0 || rial_row == img->height-1) || (rial_col == 0 || rial_col == img->width-1)))
            {
                red[cont] = img->pixels[rial_row][rial_col].R;
                green[cont] = img->pixels[rial_row][rial_col].G;
                blue[cont] = img->pixels[rial_row][rial_col].B;
                cont++;
            }
            repetido = 1;
        }
    }

    if (lugar == 5)
    {
        while (cont < 9)
        {
            red[cont] = img->pixels[row][col].R;
            green[cont] = img->pixels[row][col].G;
            blue[cont] = img->pixels[row][col].B;
            cont++;
        }
        
    }
    
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void new_color(Color *color, Image *img, int row, int col, int lugar){
    int red[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}, green[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}, blue[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < 9; i++)
    {
        red[i] = 0;
        green[i] = 0;
        blue[i] = 0;
    }

    if (lugar == 0)
    {
        for_normal(img, red, green, blue, col, row);

    } else{
        for_borde(img, red, green, blue, col, row, lugar);
    }

    qsort(red, 9, sizeof(int), cmpfunc);
    qsort(green, 9, sizeof(int), cmpfunc);
    qsort(blue, 9, sizeof(int), cmpfunc);


    color->R = red[4];
    color->G = green[4];
    color->B = blue[4];
    
}