#include "../puzzle/cell_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

void elegir_celda(int** values, int height, int width, celda* cel){
	//esto podria optimizarse con un random !!!!!!!!!1
	register int r = 0;
	register int c = 0;

		for (r = 0; r < height; r++)
	{
		for (c = 0; c < width; c++)
		{
			// si no tiene ningún distrito asignado, retorno su dirección

			if(values[r][c] == 0){
				cel->row = r;
				cel->col = c;
				return;
			}
		}
	}
}


int es_solucion(int* kodos, int*jack, int contador, int** values, CellType** cells, int rows, int cols, int per_disct, int n_dist){
	
	// si no se han asignado todas las celdas
	if (contador< rows*cols){
		return 0;
	}


	int empate = 0;
	int dist_ganados = 0;
	int perdidos = 0;
	// si llego acá es porque todos las celdas tenian un distrito asociado
	for (int d = 0; d < n_dist; d++)
	{
		// si la cantidad de votos a favor en el distrito d es mayor o igual a votos para ganar, sumo dist ganados
		if(kodos[d] > jack[d]){
			dist_ganados += 1;
		} else if(kodos[d]== jack[d]){
			empate += 1;
		}else{
			perdidos += 1;
		}
	}


	// si gané mas distritos de la mitad, retorno verdadero, si no, falso
	//printf("ganados %d perdidos%d empatados %d totaless %d\n", dist_ganados,perdidos,empate, n_dist);

	if (dist_ganados >perdidos)
	{

		return 1;
	}
	
	return 0;
}


void sumar_voto(int distrito, CellType **cells, int *kodos, int *jack, int r, int c){
	if (cells[r][c] == 2)
	{
		kodos[distrito - 1] += 1;
	}else if(cells[r][c]==3){
		jack[distrito - 1] += 1;
	}
}

void restar_voto(int distrito, CellType **cells, int *kodos, int *jack, int r, int c){
	if (cells[r][c] == 2)
	{
		kodos[distrito - 1] -= 1;
	}else if(cells[r][c]==3){
		jack[distrito - 1] -= 1;
	}
}


int gana_jack(int **values, int* kodos, int* jack, CellType** cells, int *per_act_dist, int n_dist, int pers_por, int r, int c, int distrito){

	int cont = 0;

	int dist_win = (n_dist / 2) + 1;
	int dist_perdidos = 0;
	int ganados = 0;
	int empatados = 0;

	// sumo voto para que sea contado
	sumar_voto(distrito, cells, kodos, jack, r, c);

	// si ya he designado mas de la mitad de los distritos, contando el que asignare ahora
  for (int i = 0; i < n_dist; i++)
  {
		if ((per_act_dist[i] == pers_por) || (i + 1 == distrito))
		{
			cont += 1;
			if(kodos[i] < jack[i]){
				dist_perdidos += 1;
			}else if(kodos[i] > jack[i]){
				ganados += 1;
			} else{
				empatados +=1 ;
			} 
		}
	}

	restar_voto(distrito, cells, kodos, jack, r, c);
	if(dist_perdidos>= ganados + (n_dist -cont)){
		return 1;
	}
	return 0;

	/* if (cont >= dist_win)
	{
    // reviso quien ha ganado, si no gana kodos, digo que el escenario no es valido
		
		if (dist_perdidos + empatados >= dist_win)
		{
			return 1;
		} 
		return 0;
	} */
	return 0;
}

int cierra(int row, int col, int** values, int distrito, int* pers_act, int pers_por, int width){
	// saco el distrito de la celda de arriba y veo si
	if (row == 0)
	{
		// si estoy en la linea 0 no tengo nada arriba.
		return 0;
	}

	int d_arriba = values[row - 1][col];

	int cont = 0;

	// si atrás mio designe algun distrito con el mismo numero del de  arriba, entonces si tiene salida
	for (int i = 0; i < col; i++)
	{
		if (values[row][i]== d_arriba)
		{
			cont += 1;
		}
	}


	
	if (col + 1 == width)
	{// si estoy en la ultima columna
		if (pers_act[d_arriba - 1] < pers_por && cont == 0)
		{
			return 1;
		}
		return 0;

	}

	int d_arriba_derecha = values[row - 1][col + 1];

	// si no tengo salida hacia la derecha  y aun no lleno mi distrito
	if (d_arriba != d_arriba_derecha && pers_act[d_arriba-1] < pers_por && cont ==0)
	{
		// cierra el paso
		return 1;
	}
	return 0;
}

int is_valido(int* celdas, int* kodos, int* jack, int** values, CellType **cells, int n_dist, int distrito, int row, int col, int* per_act_dist, int pers_por, int height, int width){
	// si ya esta lleno con personas el distrito, retorno falso
	int lista[2] = {1, -1};
	int r, c;
	if (per_act_dist[distrito - 1] == pers_por && cells[row][col]!= 0)
	{
		return 0;
	}

	// reviso si algún vecino tiene el mismo distrito asignado
	int cont = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{

			r = i == 0 ? row : row + lista[j];
			c = i == 0 ? col + lista[j] : col;
			// revisa si el par row, col pertenece al rango de la grilla
			if ((0<= r && r< height) && (0<= c && c < width)){
				if (values[r][c] == distrito)
				{
					/* si algun vecino tiene el mismo distrito, se puede conectar */
					if (!(row-1 == r && col == c))
						{
							if (cierra(row, col, values, distrito, per_act_dist, pers_por, width))
							{
								return 0;
							}
						}
						
					if (per_act_dist[distrito - 1]+1 == pers_por && cells[row][col] != 0) // si la persona que agrego cierra el distrito
					{
            // si al cerrarlo hago que gane jack, retorno que no es valido y paso a probar otra alternativa
            if (gana_jack(values, kodos, jack, cells, per_act_dist, n_dist, pers_por, row, col, distrito))
						{
							return 0;
						}
						cont += 1;
					}

				
					
					return 1;
				}
			}

		}	
	}

	// si llego acá es por que ningún vecino tenía el mismo distrito, por lo que puede ser que 
	// no haya ninguna persona asignada a ese distrito, lo cual sería valido, o que haya otro, y no sería valido
	

	if (celdas[distrito - 1] > 0)
	{ // si el dist fue asignado a alguna celda y no es vecina
		return 0;
	}

	// si llego acá, no hay ningúna celda con ese distrito, por lo que es valido

	if (cierra(row, col, values, distrito, per_act_dist, pers_por, width))
		{
			return 0;
		}
	return 1;
}

int elegir_num(int* revisados, int n_distritos){
    int elegido = rand()% n_distritos;
    while(revisados[elegido]==1){
        elegido =  rand()% n_distritos;
    }
    return elegido+1;
}

int rev(int *revisados, int n_distritos){
    int cont  = 0;
    for(int i = 0; i<n_distritos; i++){
        if(revisados[i] == 1){
            cont ++;
        }
    }
    if(cont == n_distritos){
        return 1;
    }
    return 0;
}