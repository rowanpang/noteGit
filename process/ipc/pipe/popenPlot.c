/* plot.c - example of unix pipe. Calls gnuplot graph drawing package to draw
graphs from within a C program. Info is piped to gnuplot */
/* Creates 2 pipes one will draw graphs of y=0.5 and y = random 0-1.0 */
/* the other graphs of y = sin (1/x) and y = sin x */

/* Also user a plotter.c module */
/* compile: cc -o plot plot.c plotter.c */

#include  <stdio.h>
#include  <stdlib.h>
#include  <signal.h>
#include  <math.h>
#include  <unistd.h>

static FILE *plot1, *plot2, *ashell;

static char *replot = "replot\n";


void openPlot(void)
{
    char *command1= "/usr/bin/gnuplot > /dev/null";
    char *command2= "/usr/bin/gnuplot > /dev/null";
    char *termX11 = "set terminal x11\n";

    plot1 = popen(command1, "w");
    fprintf(plot1, "%s", termX11);
    fflush(plot1);
    if(plot1 == NULL){
	exit(2);
    }
    plot2 = popen(command2, "w");
    fprintf(plot2, "%s", termX11);
    fflush(plot2);
    if(plot2 == NULL){
	exit(2);
    }
}

void doPlot(void)
{
    char *startplot1 =
	    "plot [] [0:1.1]'plot11.dat' with lines, 'plot12.dat' with lines\n";
    char *startplot2 =
	    "plot 'plot21.dat' with lines, 'plot22.dat' with lines\n";

    fprintf(plot1, "%s", startplot1);
    fflush(plot1);
    fprintf(plot2, "%s", startplot2);
    fflush(plot2);
}

void RemoveDat(void)
{
    char *deletefiles = "rm plot11.dat plot12.dat plot21.dat plot22.dat";
    ashell = popen(deletefiles, "w");
    exit(0);
}
void StopPlot(void)
{
    pclose(plot1);
    pclose(plot2);
}
void RePlot(void)
{
    fprintf(plot1, "%s", replot);
    fflush(plot1);
}


#define DEG_TO_RAD(x) (x*180/M_PI)

double drand48();
void quit();
FILE *fp1, *fp2, *fp3, *fp4, *fopen();
int end=0;

int main(int argc,char *argv[])
{
    float x;
    float y11,y12,y21,y22;

    /* open files which will store plot data */
    if( ((fp1 = fopen("plot11.dat","w")) == NULL) ||
	((fp2 = fopen("plot12.dat","w")) == NULL) ||
	((fp3 = fopen("plot21.dat","w")) == NULL) ||
	((fp4 = fopen("plot22.dat","w")) == NULL)){

	printf("Error can't open one or more data files\n");
	exit(1);
    }
    signal(SIGINT,quit); /* trap ctrl-c call quit fn */

    openPlot();
    y11 = 0.5;
    srand48(1); /* set seed */
    for(x=0;;x+=0.01){
	/* increment i forever use ctrl-c to quit prog */

	if(end){
	    break;
	}

	y12 = (float) drand48();
	if(x == 0.0){
	    y21 = 0.0;
	}else{
	    y21 = sin(DEG_TO_RAD(1.0/x));
	    y22 = sin(DEG_TO_RAD(x));
	}

	/* load files */
	fprintf(fp1,"%f %f\n",x,y11);
	fprintf(fp2,"%f %f\n",x,y12);
	fprintf(fp3,"%f %f\n",x,y21);
	fprintf(fp4,"%f %f\n",x,y22);
	/* make sure buffers flushed so that gnuplot */
	/* reads up to data file */
	fflush(fp1);
	fflush(fp2);
	fflush(fp3);
	fflush(fp4);

	/* plot graph */
	doPlot();
	sleep(1); /* sleep for short time */
    }
}

void quit()
{
    printf("\nctrl-c caught:\n Shutting down pipes\n");
    end=1;
    StopPlot();

    printf("closing data files\n");
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    printf("deleting data files\n");
    RemoveDat();
}
