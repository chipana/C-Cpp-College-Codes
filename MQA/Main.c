#include <stdio.h>
void lms(double *x, double *y, int n, double *a, double *b)
{
    int i;
    double sumYX = 0.;
    double sumX = 0.;
    double sumY = 0.;
    double sumX2 = 0.;
    double sum2X = 0.;
    for(i = 0; i < n; i++) {
        sumYX += x[i] * y[i];
        sumX += x[i];
        sumY += y[i];
        sumX2 += x[i] * x[i];
    }
    sum2X = sumX * sumX;

    *a = (sumYX - (sumX * sumY) / (double)n) / (sumX2 - sum2X / (double)n);
    *b = sumY / (double)n - *a * sumX / (double)n;
}
int main(){
    int count = 0, i = 0;
    double a, b;
    char url[]="dados3.dat", ch;
	FILE *arq;
	arq = fopen(url, "r");
	if(arq == NULL)
        printf("Erro, nao foi possivel abrir o arquivo\n");
	else
    {
        while( (ch=fgetc(arq))!= EOF )
			if(ch == '\n')
			  count++;
    }
    printf("Numero de dados: %d\n", count);
    double x[count], y[count];
    fclose(arq);
    arq = fopen(url, "r");
    if(arq != NULL)		  
        while((fscanf(arq,"%lf , %lf\n", &x[i], &y[i])) != EOF ){
            printf("Dados lidos: (%lf, %lf)\n", x[i], y[i]);
            i++;
        }
    fclose(arq);
    lms(x, y, count, &a, &b);
    printf("a: %lf \nb: %lf \n", a, b);
    printf("------------------------------------------\n");
    printf("||\tExpressao: y = %.1f x + %.1f\t||\n", a, b);
    printf("------------------------------------------\n");
    system("pause");
}
