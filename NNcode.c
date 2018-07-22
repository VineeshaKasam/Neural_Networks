#include<stdio.h>
#include<math.h>

double xvalues[20][9];
double yvalues[20];
double err[100];
double tweights[50];
int i,j,k=0,m,n;
int input=9;
int patterns;
int output=1;
int hidden=2;
int numweights;
double tinputs[20][9];
double inputs[50][50];
double ihweights[5][5];
double hbiases[100];
double hsums[20];
double houtputs[20];
double howeights[5][5];
double obiases[20];
double osums[20];
double result[20];
double weights[180];
long epoch;
double e;
double senfactor=1.01;
double *wvalues;
int u,v,w,x,z,q,r,h,o;

void computeoutputs(double xvalues[][9]);
void updateweights(double);
void setweights(double[]);
void sensitivity();
double learningrate,tlearningrate;
double sum;
int main()
{
    FILE *fpt;
    FILE *fp;
    FILE *fptr;
    FILE *ft;
    int maxepochs=10000;
    int ch,a,c,d,g,h,s,t,b,i;
    double momentum=0.005;
    double error;
    double errort=0.03;
    double negerrort=-0.05;
    double yvalues[20];
    printf("STARTING NEURAL NETWORK... \n");
    printf("Enter the number of patient records for training \n");
    scanf("%d",&patterns);
    fpt=fopen("rinputs.txt","r");
    if(fpt == NULL)
    {
        printf("cant be opened");
    }
    while(!feof(fpt))
    {
        for(i=0; i<patterns; i++)
        {
            for(j=0; j<input; j++)
            {
                fscanf(fpt," %lf",&(xvalues[i][j]));
            }
        }
    }
    fclose(fpt);
    printf("THE INPUT VALUES OF THE NEURAL NETWORK ARE:\n");
    for(i=0; i<patterns; i++)
    {
        for(j=0; j<input; j++)
        {
            printf("\t %lf ",xvalues[i][j]);
        }
    }
    cho:
	printf("enter choice 1. Train the Neural Network 2. Calculate the Risk Factor of CAD \n");
	scanf("%d",&c);
	switch(c)
	{
        case 1:
            goto label;
        case 2:
            printf("TESTING...");
            ft=fopen("testinp.txt","r");
            for(i=0;i<patterns;i++)
            {
                for(j=0;j<input;j++)
                {
                    fscanf(ft,"%lf",&tinputs[i][j]);
                }
	        }
            fclose(ft);
            setweights(tweights);
            computeoutputs(tinputs);
            goto lab;
        default:
            exit(0);
            break;
	}
	exit(0);
    label:
        printf("enter learning rate.. \n");
        scanf("%lf",&learningrate);
	numweights=(input)*patterns;
	printf("numweights are: %d \n",numweights);
	fp=fopen("weights.txt","r");
	if(fp == NULL)
	{
	    printf("cannot open weights file");
	}
	while(!feof(fp))
	{
	   for(i=0;i<numweights;i++)
	   {
	        fscanf(fp,"%lf",&(weights[i]));
	   }
	}
	fclose(fp);
	sensitivity();
	printf("The corresponding WEIGHTS are... \n");
	for(k=0;k<numweights;k++)
	{
	    printf("%lf \t",weights[k]);
	}
	setweights(weights);
	printf("The TARGET risk values of patients for training are: \n");
	fptr=fopen("rtargets.txt","r");
	if(fptr==NULL)
	{
	    printf("cannot open targets file");
	}
	while(!feof(fptr))
	{
        for(i=0;i<patterns;i++)
        {
            fscanf(fptr,"%lf",&yvalues[i]);
        }
	}
	fclose(fptr);
	for(i=0;i<patterns;i++)
	{
	    printf("%lf\t",yvalues[i]);
	}
	printf("beginning training... \n");
	for(epoch=1; epoch<maxepochs; epoch++)
	{
	    if(epoch>=1)
		    e=error;
		computeoutputs(xvalues);
		w=0;
		for(u=0,v=0;u<patterns;u++,v++)
		{
            err[w]=yvalues[v]-result[u];
            w++;
		}
		sum=0;
		for(x=0;x<patterns;x++)
		{
            sum+=err[x];
		}
		error=(sum/patterns);
		printf("error obtained is: %lf \n",error);
		if(epoch >2500)
		{
		    if(error>negerrort)
			{     
                if(error < errort)
                {
                    printf("training is sucessfull at %ld epoch \n",epoch);
                    tlearningrate=learningrate;
                    printf("learningrate : %lf \n",tlearningrate);
                    h=0;
                    for(s=0;s<numweights;s++)
                    {
                        tweights[h]=weights[s];
                        h++;
                    }
                    break;
		        }
		    }
		}
		printf("Error is greater than the Threshold \n");
		if(epoch < 3000)
		{
		    if(epoch!=1 && e<error)
			learningrate=learningrate-0.0001;
		    else
			learningrate=learningrate+0.02;
		    if(learningrate<1)
		    {
                  printf("MSG: since epochs are quite less in number ");
                  printf("(if) learning rate has been changed to %lf...  \n",learningrate);
		    }
		    else
		    {
                   while(learningrate>=1)
                   {
                        learningrate=learningrate-((epoch/10)*0.01);
                   }
                   printf("(else)learning rate has been changed to %lf...\n",learningrate);
		    }
		}
		else
		{
		    updateweights(momentum);
		    printf("MSG: since epochs are comparatively greater we update weights... \n");
		    setweights(weights);
		}
	}
    if(epoch>=maxepochs)
    {
    printf("------maximum epochs reached------");
    getch();
    exit(0);
    }
    goto cho;
    lab:
    getch();
	return 0;
}

void setweights(double weights[])
{
     k=0;
     numweights=input*patterns;
     for(i=0;i<(input*patterns);i++)
     {
        for(j=0;j<hidden;j++)
        {
            ihweights[i][j]=weights[k++];
        }
     }
     k=0;
     for(i=0;i<hidden;i++)
     {
        hbiases[i]=weights[k++]-0.05;
        printf("Hidden bias is : %lf \n",hbiases[i]);
     }

     for(i=0;i<hidden;++i)
     {
        for(j=0;j<patterns;++j)
        {
            howeights[i][j]=weights[k++];
        }
     }
     for(i=0;i<patterns;++i)
     {
        obiases[i]=weights[k++]-0.05;
        printf("obiases are  %lf \n ",obiases[i]);
     }
}

void computeoutputs(double xvalues[20][9])
{
     int l,p;

     for(i=0;i<patterns;i++)
        hsums[i]=0.0;

     for(i=0;i<patterns;i++)
        osums[i]=0.0;

     for(l=0,p=0;l<patterns;l++,p++)
     {
        for(i=0;i<4;i++)
        {
            inputs[p][i]=xvalues[l][i];
        }
     }
     for(p=0;p<patterns;p++)
     {
       for(j=0;j<hidden;j++)
        {
            for(i=0;i<input;i++)
            {
                hsums[p]+=inputs[p][i] * ihweights[i][j];
            }
        }
     }
     for(i=0;i<patterns;i++)
     {
        hsums[i]+= hbiases[i];
        hsums[i] -= (tanh(hsums[i]));
     }

     for(j=0;j<patterns;j++)
     {
        for(i=0;i<hidden;i++)
        {
            osums[j] += hsums[i] * howeights[i][j];
        }
     }
     for(i=0;i<patterns;i++)
     {
        osums[i] += obiases[i];
        osums[i] += 1.0/(1.0 + exp(-osums[i]));
     }
     for(m=0,n=0; m<patterns; m++,n++)
     {
        result[n]=osums[m]*learningrate;
        while(result[n]>1)
        {
            result[n]--;
        }
     }
     for(n=0;n<patterns;n++)
     {
        printf("RISK calculated for patient %d is %lf  \n",n,result[n]);
     }
 }


void updateweights(double momentum)
 {
    printf("Updating the weights \n");
    for(z=0; z<numweights; z++)
    {
        weights[z]+=momentum;
        if(weights[z] >=1.0000)
        {
            if(fmod(weights[z],2) == 0)
            {
                while(weights[z] >=1.0000)
                {
                    weights[z]-=((double)((epoch+1) / 50000)*momentum);
                }
            }
            else
            {
                while(weights[z] >=1.0000)
                {
                    weights[z]-=((double)((epoch+1) / 20000)*momentum);
                }
            }
	    }
    }
 }

void sensitivity()
{
    for(h=0,o=0; h<numweights; h++)
    {
        if(h==4*o)
        {
            weights[h]=weights[h]*senfactor;
            printf("Updation of weights of sensitive parameter: %lf \n",weights[h]);
            o++;
        }
    }
}
