#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int binary_search();
void display_class();
void display_zone();
int override();
int priority();
void assign_security();
int compare();
void rmv();
int search();
void select_place();
int mergesort();
void merge();
void quicksort();
void graphviz();

typedef struct zone
{
    int z_id;
    int z_num_node;
    int iso_patt[8];
    int num_i_p;
    int pri[8];
    int node[100];
}s_z;
typedef struct classs
{
    int c_id;
    int c_num_zone;
    struct zone zone_arr[100];
    int ip[8];
    int num_ip;
    int sum;
}s_c;

int rem_zone[10];
int rem_node[10];
int Z1;
int Z2;
int S_=0;
int kip[8];
int rf=0;
int c_rf=1;
int fire=1;
int ipsec=1;
int ids=1;
int nat=1;

void main()
{
	FILE *fp;
	char ch;
	int i=-1,x,c=0,count=0,q,flag=0,s=0,flag11=0,f=0,k,k1,num_node=0,n,num_class,sub_flag,num_zone;
	char cc[10];
	remove("placement.txt");
	remove("security.txt");
	printf("\n\n\tEnter your csv filename : ");
	scanf("%s",cc);
	fp=fopen(cc,"r");
	if (fp == NULL)
	{
        fprintf(stderr, "Can't open input file in.list!\n");
        exit(1);
    }
    while(ch!='$')
	{
	    ch = fgetc(fp);
	    printf("%c",ch);
        if(ch !=',')
        c++;
	}
	rewind(fp);
    ch = ' ';
	s_z zone_arr[c];
	int v;
    while(ch!='$')
	{
	    ch =  fgetc(fp);
        if(flag==0)
        {
            fscanf(fp,"%d",&x);
            flag=1;
            i++;
            zone_arr[i].z_id=x;
            n=0;
            k=0;
            count=0;
            if(x!=-1)
                num_node++;
        }
        else if(flag11==0)
        {
            fscanf(fp,"%d",&x);
            count++;
            zone_arr[i].iso_patt[k]=x;
            if(zone_arr[i].iso_patt[k]==0)
            {
                flag11=1;
                zone_arr[i].num_i_p = count-1;
                if(zone_arr[i].num_i_p!=0)
                {
                    quicksort(zone_arr[i].iso_patt,0,zone_arr[i].num_i_p-1);
                    int m;
                    if(zone_arr[i].num_i_p!=0)
                    {
                        for(m=0;m<zone_arr[i].num_i_p;m++)
                        {
                            zone_arr[i].pri[m] = priority(zone_arr[i].iso_patt[m]);
                        }
                    }
                    compare(zone_arr,i);
                }
            }
            k++;
        }
        else
        {
            zone_arr[i].iso_patt[k]=0;
            fscanf(fp,"%d",&x);
            if(x==-1)
            {
                flag=0;
                flag11=0;
            }
            else
            {
                zone_arr[i].node[n]=x;
                n++;
                zone_arr[i].z_num_node++;
            }
        }
    }
    fclose(fp);
    FILE *qq;
    qq=fopen("placement.txt","a+");
    fprintf(qq,"graph structs \n{\nrotate=180;");
    fclose(qq);

    display_zone(zone_arr,num_node);
    //form classes with these iso patt and zones ------------------------------------------------
    s_c class_arr[num_node];
	int flag1,flag2,next_i,cnt=0;
    num_class=0;
    for(i=0;i<num_node;i++)
    {
        //initialinzing----------------------------------------------------------
        x=0;
        class_arr[i].c_id=i+1;
        class_arr[i].c_num_zone=1;
        class_arr[i].zone_arr[x++] = zone_arr[i];
        class_arr[i].sum = zone_arr[i].z_num_node;
        num_class++;
        //--------------------------------------------------------------------------------
        count=0;
        int k_flag;
        //element to be searched is zone_arr[i].z_id in all the corresponding next_i
        for(next_i=0;next_i<num_node;next_i++)
        {
            if(next_i!=i && zone_arr[i].z_num_node >= zone_arr[next_i].z_num_node && zone_arr[next_i].z_num_node !=0  && zone_arr[next_i].num_i_p !=0 && zone_arr[i].num_i_p !=0)
            {
                //checking if zone_arr[i].z_id is in the corresponding zone_arr[next_i].node[next_j]
                flag1 = binary_search(zone_arr[next_i].node,zone_arr[next_i].z_num_node,zone_arr[i].z_id);
                //checking if zone_arr[next_i].z_id is in the corresponding zone_arr[i].node[j]
                flag2 = binary_search(zone_arr[i].node,zone_arr[i].z_num_node,zone_arr[next_i].z_id);
                //evaluating---------------------------
                if(flag1==1 || flag2 == 1)
                {
                    //not possible ...... do nothing
                }
                else if(flag1==0 && flag2==0)
                {
                    //calculate the existence of all nodes of next_i in i
                    for(k=0;k<zone_arr[next_i].z_num_node;k++)//modification from k<zone_arr[next_i].z_num_node-1
                    {
                        sub_flag = binary_search(zone_arr[i].node, zone_arr[i].z_num_node, zone_arr[next_i].node[k]);
                        if(sub_flag==1)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(sub_flag==1)
                    {
                        class_arr[i].c_num_zone= class_arr[i].c_num_zone+1;
                        class_arr[i].zone_arr[x++] = zone_arr[next_i];
                        class_arr[i].sum = class_arr[i].sum + zone_arr[next_i].z_num_node;
                    }
                }
            }
            else
            {
                count++;
                continue;
            }
        }
        assign_security(class_arr,i);
    }
    //-------------------------------------------------------------------------------------------
    //Displaying the class begins-----------------------------------------------------------------
    printf("\n\n\tYour classes formed are as follows : ");
    display_class(&class_arr,num_class);
    //--------------------------------------------------------------------------------------------
    do{
        //sorting of classes--------------------------------------------------------------------------
        S_ = mergesort(class_arr,0,num_class-1);
        //--------------------------------------------------------------------------------------------
        //select first element from sorted array------------------------------------------------------
        for(i=0;i<10;i++)
        {
            rem_zone[i] = 0;
            rem_node[i] = 0;
        }
        Z1=0;
        Z2=0;
        select_place(class_arr,num_class);
        //--------------------------------------------------------------------------------------------
        //remove the zones from the S'----------------------------------------------------------------
        rmv(class_arr[0],class_arr,zone_arr,&num_class,num_node);
        //--------------------------------------------------------------------------------------------
    }while(S_ == 0);//goto step 2 if S' is non empty*/

    FILE *o3;
    o3=fopen("placement.txt","a+");
    int p=1;
    while(p!=c_rf)
    {
        fprintf(o3,"\nstructr%d [ shape=circle,style=filled,color=black,width=\"0.3\",label=\"X\",fontcolor=\"white\"]; ",p);
        p++;
    }
    p=1;
    while(p!=fire)
    {
        fprintf(o3,"\nstructf%d [shape=record,label=\"{ {| |}|{| |} }\",style=\"filled\",fillcolor=\"orange:brown\"]; ",p);
        p++;
    }
    p=1;
    while(p!=ipsec)
    {
        fprintf(o3,"\nstructips%d [ shape=rectangle,style=\"rounded,filled\",color=lightblue,width=\"0.3\",label=\"IPSec\"]; ",p);
        p++;
    }
    p=1;
    while(p!=ids)
    {
        fprintf(o3,"\nstructids%d [ shape=polygon,color=red,fontcolor=white,style=\"filled\",width=\"0.3\" sides=10,peripheries=2,label=\"IDS\"]; ",p);
        p++;
    }
    p=1;
    while(p!=nat)
    {
        fprintf(o3,"\nstructnat%d [ shape=diamond,style=\"rounded,filled\",fillcolor=\"orange:yellow\",label=\"NAT\"]; ",p);
        p++;
    }
    p=0;
    while(p!=c_rf)
    {
        if(p==0)
        {
            fprintf(o3,"\n{rank=same;");
            p++;
        }
        else
        {
            if(p!=c_rf)
            {
                fprintf(o3,"structr%d,",p);
                p++;
                if(p==c_rf)
                {
                    fseek(o3,-1,SEEK_END);
                    ftruncate(fileno(o3), ftell(o3));
                    fprintf(o3,"};");
                }
            }
        }

    }
    p=0;

    num_node++;
    while(p!=(num_node))
    {
        if(p==0)
        {
            fprintf(o3,"\n{rank=same;");
            p++;
        }
        else
        {
            if(p!=num_node)
            {
                fprintf(o3,"struct%d,",p);
                p++;
                if(p==num_node)
                {
                    fseek(o3,-1,SEEK_END);
                    ftruncate(fileno(o3), ftell(o3));
                    fprintf(o3,"};");
                }
            }
        }

    }
    p=0;

    fclose(o3);

    o3=fopen("placement.txt","a+");
    fprintf(o3,"\n\n}");
    fclose(o3);
    system("C:/Users/!..Swapnil..Aryan..!/Desktop/Finalize/placement.txt");
}
//function to display zones----------------------------------------------------------------------
void display_zone(s_z *zone_p,int num_node)
{
    int i,j;
    FILE *o1,*o2;
    o1 = fopen("zone.txt","w");
    o2 = fopen("placement.txt","a+");
	if (o1 == NULL||o2==NULL)
	{
        fprintf(stderr, "Can't open input file in.list!\n");
        exit(1);
    }
    fprintf(o2,"\n");
    for(i=0;i<num_node;i++)
    {
        printf("\n Z_%d = { ",(zone_p+i)->z_id);
        fprintf(o1,"\n Z_%d = { ",(zone_p+i)->z_id);

        fprintf(o2,"\nstruct%d [shape=box,peripheries=2,color=darkgreen,width=\"0.3\", style=\"filled\",fontcolor=white,label=\"%d\"];",(zone_p+i)->z_id,(zone_p+i)->z_id);
        for(j=0;j<(zone_p+i)->z_num_node;j++)
        {
            if(zone_p[i].node[j]!=-1)
            {
                printf("%d,",(zone_p+i)->node[j]);
                fprintf(o1,"%d,",(zone_p+i)->node[j]);
            }
        }
        fseek(o1, -1, SEEK_END);
        ftruncate(fileno(o1), ftell(o1));
        printf("\b }\n\n");
        fprintf(o1," }\n\n");
    }
    fprintf(o2,"\n");
    fclose(o1);
    fclose(o2);
}

//-----------------------------------------------------------------------------------------------
//function to select_place
void select_place(s_c class_arr[],int num_class)
{
    int i,max,id;
    //select 1st element from S'
    max=class_arr[0].zone_arr[0].z_num_node;
    id=0;
    for(i=1;i<class_arr[0].c_num_zone;i++)
    {
        if(class_arr[0].zone_arr[i].z_num_node > max)
        {
            max = class_arr[i].zone_arr[i].z_num_node;
            id = i;
        }
        else
        {
            continue;
        }
    }
    //printing in file-------------------
    FILE *o2,*o3;
    o2 = fopen("security.txt","a+");
    o3 = fopen("placement.txt","a+");
	if (o2 == NULL || o3 == NULL)
	{
        fprintf(stderr, "Can't open input file in.list!\n");
        exit(1);
    }
    printf("\n\nPlacing security between ( ");
    fprintf(o2,"\n\nPlacing security between ( ");
    int z1=0,z2=0;

    int f4,f2,f1,f3;

    f2 = binary_search(class_arr[0].ip,class_arr[0].num_ip,2);
    f4 = binary_search(class_arr[0].ip,class_arr[0].num_ip,4);
    f1 = binary_search(class_arr[0].ip,class_arr[0].num_ip,1);
    f3 = binary_search(class_arr[0].ip,class_arr[0].num_ip,3);

    for(i=0;i<class_arr[0].c_num_zone;i++)
    {
        printf("%d,",class_arr[0].zone_arr[i].z_id);
        fprintf(o2,"%d,",class_arr[0].zone_arr[i].z_id);
        //saving it bcz we need it to be deleted later in the program
        fprintf(o3,"\nstruct%d--structr%d;",class_arr[0].zone_arr[i].z_id,c_rf);
        rem_zone[z1++] = class_arr[0].zone_arr[i].z_id;
        if(i==class_arr[0].c_num_zone-1)
        {
            fseek(o2, -1, SEEK_END);
            ftruncate(fileno(o2), ftell(o2));
            fprintf(o2," ) -- [ ");
        }
    }
    fclose(o3);

    printf("\b )--[ ");
    int k,seq[5],e=0;
    k=0;
    while(class_arr[0].ip[k]!=0)
    {
        printf("%d,",class_arr[0].ip[k]);
        seq[k]=class_arr[0].ip[k];
        fprintf(o2,"%d,",class_arr[0].ip[k]);
        k++;
        e=k;
    }
    graphviz(f1,f2,f3,f4,c_rf);
    printf("\b ] -- ( ");

    o3 = fopen("placement.txt","a+");
    for(i=0;i<class_arr[0].zone_arr[id].z_num_node;i++)
    {
        printf("%d,",class_arr[0].zone_arr[id].node[i]);
        fprintf(o2,"%d,",class_arr[0].zone_arr[id].node[i]);

        fprintf(o3,"\n");
        fprintf(o3,"\nstructr%d--struct%d;",c_rf,class_arr[0].zone_arr[id].node[i]);

        rem_node[z2++] = class_arr[0].zone_arr[id].node[i];
        if(i==class_arr[0].zone_arr[id].z_num_node-1)
        {
            fseek(o2, -1, SEEK_END);
            ftruncate(fileno(o2), ftell(o2));
            fprintf(o2," )");
        }
    }
    c_rf++;
    printf("\b )\n");
    fclose(o2);
    fclose(o3);
    rem_zone[z1] = -1;
    rem_node[z2] = -1;
    Z1 = z1;
    Z2 = z2;
}
//----------------------------------------------------------------------------------------------
//funtion to remove ---------------------------------------------------------------------
void rmv(s_c rem,s_c class_arr[],s_z zone_arr[],int *num_class,int num_node)
{
    int i,count,k;
    //removing the first class -----------------------------------------------
    count=0;
    for(i=0;i<=*num_class-2;i++)
    {
        class_arr[i] = class_arr[i+1];
        count++;
    }
    *num_class = count;
    //removal of 1st class ends------------------------------------------------
    //removing all the zones in rem_zone from next class onwards----------------
    for(i=0;i<*num_class;i++)
    {
        for(k=0;k<class_arr[i].c_num_zone;k++)
        {
            int flag;
            quicksort(rem_zone,0,Z1-1);
            flag = binary_search(rem_zone,Z1,class_arr[i].zone_arr[k].z_id);//modifaction from binary_search()
            if(flag == 1)
            {
                //remove the zones
                int x,f=0;
                for(x=k;x<class_arr[i].c_num_zone;x++)
                {
                    f=1;
                    class_arr[i].zone_arr[x] = class_arr[i].zone_arr[x+1];
                }
                if(f==1)
                {
                    class_arr[i].c_num_zone--;
                    k--;
                }
            }
        }
    }
    //end of removal of same zone------------------------------------------------------------------------
    //removal of empty classes---------------------------------------------------------------------------
    for(i=0;i<*num_class;i++)
    {
        if(class_arr[i].c_num_zone == 0)
        {
            for(k=i;k<*num_class-1;k++)
            {
                class_arr[k] = class_arr[k+1];
            }
            i--;
            *num_class = *num_class - 1;
        }
    }
    //end of removal of empty classes--------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------------------------
//function to sort the classes-------------------------------------------------------------------
int mergesort(s_c class_p[], int low, int high)
{
    int mid,flag=0;
    if(low<high)
    {
        flag=1;
        mid=(low+high)/2;
        mergesort(class_p,low,mid);
        mergesort(class_p,mid+1,high);
        merge(class_p,low,high,mid);
    }
    if(flag==0)
    {
        S_ = 1;
        return S_;
    }
return(0);
}
void merge(s_c a[], int low, int high, int mid)
{
    int l1,r1,i,j;
    s_c b[20];
    l1=low;
    r1=mid+1;
    i=low;
    while((l1<=mid) && (r1<=high))
    {
        if(a[l1].sum > a[r1].sum )
        {
            b[i]=a[l1];
            l1=l1+1;
            i=i+1;
        }
        else
        {
            b[i]=a[r1];
            r1=r1+1;
            i=i+1;
        }
    }
    while(l1<=mid)
    {
        b[i]=a[l1];
        l1=l1+1;
        i=i+1;
    }
    while(r1<=high)
    {
        b[i]=a[r1];
        r1=r1+1;
        i=i+1;
    }
    for(j=low;j<i;j++)
        a[j]=b[j];
}
//-----------------------------------------------------------------------------------------------
//function to display class-----------------------------------------------------------------------
void display_class(s_c *class_p,int num_class)
{
    int i,j,k;
    for(i=0;i<num_class;i++)
    {
        k=0;
        printf("\n\n\t c_%d = { ",(class_p+i)->c_id);
        for(j=0;j<(class_p+i)->c_num_zone;j++)
        {
            printf("Z_%d, ",(class_p+i)->zone_arr[j].z_id);
        }
        printf("\b\b }");
        printf("  Having isolation patterns [ ");
        while((class_p+i)->ip[k] != 0)
        {
            printf("%d,",(class_p+i)->ip[k]);
            k++;
        }
        printf("\b ]");
        printf("\t where c_%d is S_",(class_p+i)->c_id);
        for(j=0;j<(class_p+i)->c_num_zone;j++)
        {
            printf("%d",(class_p+i)->zone_arr[j].z_id);
        }
    }
}
//-----------------------------------------------------------------------------------------------
int binary_search(int a[],int n, int key)
{
   int flag=0,low,mid,high;
   high=n-1;
   low=0;
   while(low<=high)
   {
       mid = (low+high)/2;
       if(key == a[mid])
       {
           flag=1;
           return flag;
       }
       else if(key<a[mid])
       {
           high=mid-1;
       }
       else
       {
           low=mid+1;
       }
   }
   return flag;
}
//------------------------------------------------------------------------------------------------
//priority function-----------------------------------------------------------------------------
int priority(int n)
{
    switch(n)
    {
        case 1: return 9;
                break;
        case 2: return 9;
                break;
        case 3: return 8;
                break;
        case 4: return 7;
                break;
        default: return 0;
                 break;
    }
}
//-----------------------------------------------------------------------------------------
//compare function-----------------------------------------------------------------------
int compare(s_z zone_arr[],int index)
{
    int i=0,f1,f2,f4,n;
    n = zone_arr[index].num_i_p;
    f1=binary_search(zone_arr[index].iso_patt,n,1);
    f2=binary_search(zone_arr[index].iso_patt,n,2);
    f4=binary_search(zone_arr[index].iso_patt,n,4);
    if(f1==1 && f2==1)
    {
        while(i<n)
        {
           zone_arr[index].iso_patt[i] = zone_arr[index].iso_patt[i+1];
           i++;
        }
        i=0;
    }
    else if(f1==1 && f4==1)
    {
        zone_arr[index].iso_patt[0]=4;
        zone_arr[index].iso_patt[n-1]=0;
    }
    return 0;
}
//------------------------------------------------------------------------------------------
//funtion to check if the isolation pattern can be overridden or not ----------------------------
int override(s_z zone_arr[],int index,int a,int pri)
{
    int i;
    for(i=0;i<zone_arr[index].num_i_p;i++)
    {
        if(zone_arr[index].iso_patt[i] != a)
        {
            if(zone_arr[index].pri[i] == pri)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}
//------------------------------------------------------------------------------------------
//function to assign security ----------------------------------------------------
void assign_security(s_c class_arr[],int index)
{
    int i,j=0,z;
    int max = class_arr[index].zone_arr[0].num_i_p;
    for(i=1;i<class_arr[index].c_num_zone;i++)
    {
        if(class_arr[index].zone_arr[i].num_i_p > max)
        {
            max = class_arr[index].zone_arr[i].num_i_p;
            j=i;
        }
    }
    int count=0;
    for(i=0;i<max;i++)
    {
        class_arr[index].ip[i] = class_arr[index].zone_arr[j].iso_patt[i];
        kip[i]=class_arr[index].ip[i];
        count++;
    }
    class_arr[index].ip[i] = 0;
    kip[i]=0;
    count++;
    class_arr[index].num_ip=count;
    int k,f;
    z=i;
    for(i=0;i<class_arr[index].c_num_zone;i++)
    {
        for(k=0;k<class_arr[index].zone_arr[i].num_i_p;k++)
        {
            f = binary_search(kip,count,class_arr[index].zone_arr[i].iso_patt[k]);
            if(f==1)
            {
                // do nothing
            }
            else if(f==0)
            {
                count++;
                kip[z] = class_arr[index].zone_arr[i].iso_patt[k];
                z++;
                kip[z]=0;
            }
        }
    }
    k=0;
    count=0;
    while(kip[k]!=0)
    {
        class_arr[index].ip[k]=kip[k];
        k++;
        count++;
    }
    kip[k]=0;
    class_arr[index].ip[k]=0;
    int m=0;
    count=override2(kip,count);
    m=0;
    while(m<count)
    {
        class_arr[index].ip[m] = kip[m];
        m++;
    }
    kip[m]=0;
    class_arr[index].ip[count]=0;
    class_arr[index].num_ip=count+1;
    quicksort(class_arr[index].ip,0,count-1);
}
//-------------------------------------------------------------------------------------------
//funtion to check if the isolation pattern can be overridden or not ----------------------------
int override2(int kip[],int n)
{
    int i=0,f1,f2,f4;
    f1=binary_search(kip,n,1);
    f2=binary_search(kip,n,2);
    f4=binary_search(kip,n,4);
    if(f1==1 && f2==1)
    {
        while(i<n-1)
        {
           kip[i] = kip[i+1];
           i++;
        }
        i=0;
        kip[n-1]=0;
    }
    else if(f1==1 && f4==1)
    {
        kip[0]=4;
        kip[n-1]=0;
    }
    i=0;
    while(kip[i]!=0)
    {
        i++;
    }
    return i;
}
//------------------------------------------------------------------------------------------
//sorting rem_zone using quick sort-------------------------------------------------------------------
void quicksort(int rem_zone[],int first,int last)
{
    int pivot,j,temp,i;
    if(first<last)
    {
        pivot=first;
        i=first;
        j=last;
        while(i<j)
        {
            while(rem_zone[i]<=rem_zone[pivot]&&i<last)
                i++;
            while(rem_zone[j]>rem_zone[pivot])
                j--;
            if(i<j)
            {
                temp=rem_zone[i];
                rem_zone[i]=rem_zone[j];
                rem_zone[j]=temp;
            }
        }
        temp=rem_zone[pivot];
        rem_zone[pivot]=rem_zone[j];
        rem_zone[j]=temp;
        quicksort(rem_zone,first,j-1);
        quicksort(rem_zone,j+1,last);
    }
}
//----------------------------------------------------------------------------------------------------
void graphviz(int f1,int f2, int f3, int f4,int rt)
{
    FILE *o3;
    o3=fopen("placement.txt","a+");
    if(f1==1)//if firewall
    {
        if(f3==1)//firewall and IDS
        {
            //no. of routers requierd 3, firewall 1 and 1 ids
            fprintf(o3,"structr%d--structf%d--",rt,fire);
            rt++;
            fire++;
            fprintf(o3,"structr%d--structids%d--",rt,fire,ids);
            fire++;
            ids++;
            rt++;
            fprintf(o3,"structr%d;",rt);
        //    rt++;
        }
        else
        {
            fprintf(o3,"structr%d--structf%d--",rt,fire);
            rt++;
            fire++;
            fprintf(o3,"structr%d;",rt);
            //rt++;
        }
    }
    else if(f2==1)//ipsec
    {
        if(f3==1)
        {
            if(f4==1)
            {
                fprintf(o3,"\nstructr%d--structips%d--structnat%d--",rt,ipsec,nat);
                rt++;
                ipsec++;
                nat++;
                fprintf(o3,"structr%d--structids%d--",rt,ids);
                rt++;
                ids++;
                fprintf(o3,"structr%d;",rt);
                //rt++;
            }
            else
            {
                fprintf(o3,"\nstructr%d--structips%d--",rt,ipsec);
                rt++;
                ipsec++;
                fprintf(o3,"structr%d--structids%d--",rt,ids);
                rt++;
                ids++;
                fprintf(o3,"structr%d;",rt);
                //rt++;
            }
        }
        else
        {
            fprintf(o3,"\nstructr%d--structips%d--",rt,ipsec);
            rt++;
            ipsec++;
            fprintf(o3,"structr%d;",rt);
            //rt++;
        }
    }
    else if(f3==1)
    {
        if(f4==1)
        {
            printf("\nf3 f4 %d    " ,rt);
            fprintf(o3,"\nstructr%d--structnat%d--",rt,nat);
            rt++;
            nat++;
            fprintf(o3,"structr%d--structnat%d--structids%d--",rt,nat,ids);
            rt++;
            fprintf(o3,"structr%d;",rt);
            nat++;
            ids++;
        }
        else
        {
            fprintf(o3,"\nstructr%d--structids%d--",rt,ids);
            rt++;
            ids++;
            fprintf(o3,"structr%d;",rt);
            //rt++;
        }
    }
    else if(f4==1)
    {
        fprintf(o3,"\nstructr%d--structnat%d--",rt,nat);
        rt++;
        nat++;
        fprintf(o3,"structr%d;",rt);
        //rt++;
    }
    c_rf = rt;
    fclose(o3);
}
