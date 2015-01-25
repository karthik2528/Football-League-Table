#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

void table();
void insert();
void most_goals();
void team_matches();

struct match_structure
{
    char team1[20];
    char team2[20];
    int goals1;
    int goals2;
};

struct st_team_stats
{
    char name[20];
    int pts;
    int gf;
    int ga;
};

main() 
{
    struct match_structure match;
    
    FILE *fp_matches_txt;
    FILE *fp_matches_bin;
    
    //int i=0;
    
    int loop=1,choice;
    
    while(loop==1)
    {
        fp_matches_txt=fopen("matches.txt","r");
        fp_matches_bin=fopen("matches.bin","wb");
        
        while(fscanf(fp_matches_txt,"%[^,],%[^,],%i,%i",match.team1,match.team2,&match.goals1,&match.goals2) != EOF)
        {
            fwrite(&match,sizeof(match),1,fp_matches_bin);
        }
        
        fclose(fp_matches_txt);
        fclose(fp_matches_bin);
        
        //Menu Text
        printf("\n==============\n");
        printf("   Main Menu\n");
        printf("==============\n");
        printf("\n:: Options ::\n");
        
        printf("\n1. - Show the Table");
        printf("\n2. - Insert a New Result");
        printf("\n3. - Display Match with Most Goals");
        printf("\n4. - Display Team Match History");
        printf("\n5. - Exit");
        
        //Read in menu option choice
        printf("\n\nChoose an Option: ");
        scanf("%d",&choice);
        flushall();
        
        //Switch statement to call the functions associated with each option
        switch(choice)
        {
            /*Read in 6 numbers and error check them*/
            case 1:
            {
                table();
                break;
            }
            case 2:
            {
                insert();
                break;
            }
            case 3:
            {
                most_goals();
                break;
            }
            case 4:
            {
                team_matches();
                break;
            }
            case 5:
            {
                printf("\nExiting now...");
                Sleep(2000);
                loop=0;
                break;
            }
            default:
            {
                printf("Invalid Option Input!");
                break;
            }
        }//end switch
    }//end while()
}//end main()

void table()
{
    struct match_structure match;
    FILE *fp_matches_bin;
    fp_matches_bin=fopen("matches.bin","rb");
    int i;
    int j;
    int pos;
    int pos1;
    int pos2;
    
    struct st_team_stats teams[10];
    struct st_team_stats temp;
    
    strcpy(teams[0].name,"Barcelona");
    strcpy(teams[1].name,"Bayern Munich");
    strcpy(teams[2].name,"Chelsea");
    strcpy(teams[3].name,"Inter Milan");
    strcpy(teams[4].name,"Juventus");
    strcpy(teams[5].name,"Liverpool");
    strcpy(teams[6].name,"Manchester Utd");
    strcpy(teams[7].name,"Porto");
    strcpy(teams[8].name,"PSG");
    strcpy(teams[9].name,"Real Madrid");
    
    for(i=0;i<10;i++)
    {
        teams[i].pts=0;
        teams[i].gf=0;
        teams[i].ga=0;
    }
    
    while(feof(fp_matches_bin)==0)
    //for(i=0;i<size;i++)
    {
        fread(&match,sizeof(match),1,fp_matches_bin);
        
        //Get positions in teams[] array for the two teams involved in the match
        for(j=0;j<10;j++)
        {
            if(strstr(match.team1,teams[j].name)!=NULL)
            {
                pos1=j;
            }
            if(strstr(match.team2,teams[j].name)!=NULL)
            {
                pos2=j;
            }
        }
        
        for(j=0;j<2;j++)
        {
            //Run input of data once for each team ie. their position in teams[]
            if(j==0)
            {
                pos=pos1;
            }
            if(j==1)
            {
                pos=pos2;
            }
            
            // (Team 1) Wins
            if(match.goals1 > match.goals2 && strstr(match.team1,teams[pos].name))
            {
                teams[pos].pts+=3;
                teams[pos].gf+=match.goals1;
                teams[pos].ga+=match.goals2;
            }
            // (Team 2) Wins
            if(match.goals1 < match.goals2 && strstr(match.team2,teams[pos].name))
            {
                teams[pos].pts+=3;
                teams[pos].gf+=match.goals2;
                teams[pos].ga+=match.goals1;
            }
            
            // (Team 1) Loses
            if (match.goals1 < match.goals2 && strstr(match.team1,teams[pos].name))
            {
                teams[pos].gf+=match.goals1;
                teams[pos].ga+=match.goals2;
            }
            // (Team 2) Loses
            if (match.goals1 > match.goals2 && strstr(match.team2,teams[pos].name))
            {
                teams[pos].gf+=match.goals2;
                teams[pos].ga+=match.goals1;
            }
            
            // (Team 1/2) Draws
            if (match.goals1==match.goals2)
            {
                teams[pos].pts+=1;
                teams[pos].gf+=match.goals1;
                teams[pos].ga+=match.goals2;
            }
            
        }
    }
    
    //Bubble sort for Table
    for(i=0;i<10-1;i++)
    {
        for(j=0;j<10-1;j++)
        {
            if(teams[j+1].pts>teams[j].pts)
            {
                temp=teams[j];
                teams[j]=teams[j+1];
                teams[j+1]=temp;
            }
        }
    }
    printf("\n============");
    printf("\nLeague Table");
    printf("\n============\n");
    printf("\nPosition    Team            PTS    GF     GA");
    printf("\n=============================================");
    for(i=0;i<10;i++)
    {
        printf("\n%-2i          %-15s %2d     %2d     %2d",i+1,teams[i].name,teams[i].pts,teams[i].gf,teams[i].ga);
    }
    printf("\n");
    
    fclose(fp_matches_bin);
    
}//end table()

void insert()
{
    FILE *fp_matches_txt;
    fp_matches_txt = fopen("matches.txt","a");
    
    struct match_structure match;
    
    printf("\nInsert a new result\n\n");
    //flushall();
    
    printf("Team A: ");
    gets(match.team1);
    //flushall();
    
    printf("Team B: ");
    gets(match.team2);
    //flushall();
    
    printf("Goals A: ");
    scanf("%i",&match.goals1);
    //flushall();
    
    printf("Goals B: ");
    scanf("%i",&match.goals2);
    //flushall();
    
    //fseek(fp_matches_bin,-1*(sizeof(struct match_structure)),SEEK_END);
    //fwrite(&match,sizeof(match_structure),1,fp_matches_bin);
    fprintf(fp_matches_txt,"%s,%s,%d,%d\n", match.team1,match.team2,match.goals1,match.goals2);
    fclose(fp_matches_txt);
    
    /*
    fp_matches_bin=fopen("matches.bin","rb");
    fseek(fp_matches_bin,-2*(sizeof(match_structure)),SEEK_END);
    fread(&match,sizeof(match_structure),1,fp_matches_bin);
    printf("\n\n%s %s %d %d",match.team1,match.team2,match.goals1,match.goals2);
    fseek(fp_matches_bin,-3*(sizeof(match_structure)),SEEK_END);
    fread(&match,sizeof(match_structure),1,fp_matches_bin);
    printf("\n\n%s %s %d %d",match.team1,match.team2,match.goals1,match.goals2);
    */
    
    flushall();
    
}//end insert()

void most_goals()
{
    struct match_structure temp_match,highest_match;
    FILE *fp_matches_bin;
    fp_matches_bin=fopen("matches.bin","rb");
    
    highest_match.goals1=0;
    highest_match.goals2=0;
    
    while(feof(fp_matches_bin)==0)
    //for(int i=0;i<180;i++)
    {
        fread(&temp_match,sizeof(struct match_structure),1,fp_matches_bin);
        
        if((temp_match.goals1+temp_match.goals2) > (highest_match.goals1+highest_match.goals2))
        {
            highest_match=temp_match;
        }
    }
    printf("\nThe match with the highest number of goal scored: ");
    printf("\n%s %d - %d %s\n",highest_match.team1,highest_match.goals1,highest_match.goals2,highest_match.team2);
    
    fclose(fp_matches_bin);
    
}//end most_goals()

void team_matches()
{
    struct match_structure match;
    FILE *fp_matches_bin;
    fp_matches_bin=fopen("matches.bin","rb");
    
    char search_team[20];
    printf("\nEnter a team name to display results for: ");
    gets(search_team);
    
    while(feof(fp_matches_bin)==0)
    //for(int i=0;i<180;i++)
    {
        fread(&match,sizeof(struct match_structure),1,fp_matches_bin);
        if(strstr(match.team1,search_team)!=NULL || strstr(match.team2,search_team)!=NULL)
        {
            printf("%-18s %d  -  %d %18s",match.team1,match.goals1,match.goals2,match.team2);
        }
    }
    printf("\n");
}//end matches()