// Vasile-Daniel DAN
// 30 April 2024 
#include <stdio.h>
#include <math.h>

#define E 10.0       // [V]
#define R 2.0     // [Ohm]
#define L 1.0     // [H]
#define DT 0.001    // 
#define NUM_STEPS 4000 

double derivative(double i){
    return (E-R*i)/L; 
}

int main(){
    double t[NUM_STEPS];
    double i[NUM_STEPS];
    double i_next; 

    t[0]=0.0; 
    i[0]=0.0;


    FILE *fp = NULL; 
    fp = fopen("rl_euler_output_data.txt", "w");

    if(fp == NULL){
        printf("Error opening the file");
        return 1;
    }

    for (int k = 0; k < NUM_STEPS; k++){

        printf("Time: %.2f, Current: %.4f A\n", t[k], i[k]);
        fprintf(fp, "%lf\t %lf\n", t[k], i[k]);
        i_next = i[k] +DT*derivative(i[k]);
        t [k+1] = t[k] + DT; 
        i[k+1] = i_next; 
    }


    
    // ================= GNU PLOT =========================== // 

    FILE *gnuplotPipe = NULL; 
    gnuplotPipe = popen("gnuplot -persist", "w");

    if (gnuplotPipe == NULL){
        printf("Error opening Gnuplot");
        return 1;
    }

    fprintf(gnuplotPipe, "set terminal png\n");
    fprintf(gnuplotPipe, "set output 'rl_euler_plot.png'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set title 'Behaviour of current through the inductor'\n");
    fprintf(gnuplotPipe, "set xlabel 't [s]'\n");
    fprintf(gnuplotPipe, "set ylabel 'i_L [A]'\n");
    fprintf(gnuplotPipe, "set label 'Explicit Euler method' at 1, 3\n");

    fprintf(gnuplotPipe, "plot 'rl_euler_output_data.txt' using 1:2 with lines linewidth 2 linecolor rgb 'blue'\n");

    fflush(gnuplotPipe);
    fprintf(gnuplotPipe, "exit\n");
    pclose(gnuplotPipe);


    return 0; 
}