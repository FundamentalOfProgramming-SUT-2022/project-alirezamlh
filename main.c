#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXSIZE 100
#define MAXSTRSIZE 100
#define MAXPATHSIZE 100

char first_address[100];
char undo_address[100];
char file_location[100];
char file_name[100];
char useless[100];
char clipBoard[1000000];
int EXIT=0;
FILE* file_pointer_maker(char tmp[MAXSIZE]);
void scanfcommand();
int directory_existence(const char *path);
int check_switch(char* my_switch);
int reach_to_pos(FILE* file_pointer , int line_number, int char_pos);
void scanf_str(char *);
int str_interpreter(char *);
int switch_interpret(char* a);
void file_copy(FILE* dest , FILE* src);
int check_char(char a);
void f_name_maker(char* str , int i);
void undo_helper1(FILE* file_pointer);
void undo_helper2(char* file_name);


void create_file();
void insert_str(FILE* file_pointer  , char* file_name , int char_num_from_start , char* string_to_insert);
void remove_str(FILE* file_pointer  , char* file_name, int char_pos , int char_num_to_remove , char back_or_forward);
void cat_str(FILE* file_pointer , char* file_name);
void copy_str(FILE* file_pointer , int char_num_from_start , int char_num_to_copy  ,char back_or_forward);
void cut_str(FILE* file_pointer, char* file_name , int char_num_to_cut , int char_num_from_start , char back_or_forward);
void paste_str(FILE * file_pointer , char* file_name ,int char_num_from_start);
int find_str(FILE* file_pointer , char* string_to_find  , int *word_keeper , int* index_keeper  , int* size_keeper , int* end_pos ,int  first_switch , int second_switch  , int at_num , int replace_mode , int mode);
void replace();
int grep(FILE* file_name , char * string_to_find , int *line_keeper);
void undo(FILE* file_pointer , char * file_name);
void auto_indent(FILE* file_pointer , char * file_name);
void text_comperator(FILE * fp1 , FILE * fp2);
void directory_tree(int depth , int helper);




int main(){
    getcwd(first_address , 100);
    if(!directory_existence("undo"))
        mkdir("undo");
    chdir("undo");
    getcwd(undo_address , 100);
    chdir(first_address);

    while(!EXIT){
    scanfcommand();
    }
    char a[100];
    char b[100];
   // scanf("%s" , a);
   // gets(b);
    //printf("%s\n%s\n" , a , b);
    return 0;
}


FILE* file_pointer_maker(char tmp[MAXSTRSIZE]){
    getchar();
    char first_char;
    scanf("%c" , &first_char);
    //printf("%c" , first_char);
    char file_location[MAXPATHSIZE]; //i got an error by not initializing the number of characters
    if(first_char== '/'){
    scanf("%s" , file_location);
    }
    else if(first_char=='"'){
        int i=0;
        while(1){
            scanf("%c" , file_location+i);
            if(*(file_location+i)=='"'){
                *(file_location+i)='\0';
                break;
                }
        i++;
        }
        //printf("%s" , file_location);
    }
    if(strlen(file_location)<3){
        gets(useless);
        printf("please make sure you have entered the right path");
        return NULL;
    }
    char* token= strtok(file_location , "/");

    strcpy(tmp , token);
    while(1){
        token=strtok(NULL , "/");
        if(token!=NULL){
            if(directory_existence(tmp))
            {
                chdir(tmp);
                strcpy(tmp , token);

            }
            else
            {
                gets(useless);
                printf("directory %s does not exist" , tmp);
                return NULL;
            }
        }
        else break;
    }
    FILE* file_pointer;
    file_pointer= fopen(tmp ,"r");
    if (file_pointer==NULL){
        gets(useless);
        printf("the file does not exist");
        }
return file_pointer;
}
FILE* file_pointer_maker2(char * file_location , char * tmp){
    //printf("%s" , file_location);
    char* token= strtok(file_location , "/");

    strcpy(tmp , token);
    while(1){
        token=strtok(NULL , "/");
        if(token!=NULL){
            if(directory_existence(tmp))
            {
                chdir(tmp);
                strcpy(tmp , token);

            }
            else
            {
                gets(useless);
                printf("directory %s does not exist" , tmp);
                return NULL;
            }
        }
        else break;
    }
    FILE* file_pointer;
    file_pointer= fopen(tmp ,"r");
    if (file_pointer==NULL){
        gets(useless);
        printf("the file does not exist");
        }
return file_pointer;


}

void scanfcommand(){
    char command[50];
    scanf("%s" , command);
    if(!strcmp(command ,"createfile")){
        create_file();
    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"insertstr")){
    char string_to_insert[MAXSTRSIZE];
    char string_holder[MAXSTRSIZE];
    char file_name[100];
    int char_num_from_start;
    int flag=0 , line_num , start_pos;

    if (check_switch("--file"))
       return;

    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;

    undo_helper1(file_pointer);

    if (check_switch("--str"))
        return;
    scanf_str(string_to_insert);
    str_interpreter(string_to_insert);

    if (check_switch("--pos"))
        return;
    scanf(" %d:%d" , &line_num , &start_pos);
    //printf("position:%d:%d\n" , line_num , start_pos* line_num);
    char_num_from_start=reach_to_pos(file_pointer ,line_num , start_pos);
    //printf("there are %d chars from start in this file" , char_num_from_start);
    if(char_num_from_start==-1){
        printf("this position does not exist in the file.\n");
        return;
    }
    insert_str( file_pointer , file_name ,char_num_from_start ,  string_to_insert);
    printf("the string was successfully inserted in the file.\n");
    undo_helper2(file_name);
    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"cat")){
        int flag=0;
        char tmp[MAXSTRSIZE];
        char file_name[100];
        if (check_switch("--file")) return;
        //char armanchar;
        //armanchar= getchar();
        //if(armanchar==' '){
       // printf("arman mode activated\n");
       // }
        FILE* file_pointer;
        file_pointer= file_pointer_maker(file_name);
        if(file_pointer==NULL) return;
        cat_str(file_pointer , file_name);

    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"removestr")){
    char file_name[100];
    int flag=0 , line_num , char_pos , char_num_to_remove , char_num_from_start;
    char back_or_forward;
    if (check_switch("--file")) return;
    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;

    undo_helper1(file_pointer);

    if (check_switch("--pos")) return;

    scanf("%d:%d" , &line_num , &char_pos);
    //printf("linenumber:%d\nchar_pos:%d\n" , line_num , char_pos);
    if (check_switch("-size"))
       return;
    scanf("%d" , &char_num_to_remove);
    //printf("%d\n" , char_num_to_remove);

    scanf(" -%c" , &back_or_forward);
    //printf("%c\n" , back_or_forward);
    flag=0;
    char_num_from_start= reach_to_pos(file_pointer , line_num , char_pos);
    printf("the chars from start:%d\n" , char_num_from_start);
    rewind(file_pointer);
    if(flag==1){
        printf("this position does not exist in your file.\n");
        fclose(file_pointer);
        return;
    }
        remove_str( file_pointer , file_name , char_num_from_start , char_num_to_remove , back_or_forward);
        printf("the string was successfully removed from the file.\n");
        undo_helper2(file_name);
    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"copystr")){
            char file_name[100];
    int flag=0 , line_num , char_pos , char_num_to_copy , char_num_from_start;
    char back_or_forward;
    if (check_switch("--file")) return;
    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;
    //printf("%s\n" , file_name);

    if (check_switch("--pos")) return;

    scanf("%d:%d" , &line_num , &char_pos);

    //printf("%d\n%d\n" , line_num , char_pos);
    if (check_switch("-size"))
       return;
    scanf(" %d" , &char_num_to_copy);
    //printf("%d\n" , char_num_to_copy);

    scanf(" -%c" , &back_or_forward);
   // printf("%c\n" , back_or_forward);

    char_num_from_start= reach_to_pos(file_pointer , line_num , char_pos);
   // printf("%d" , char_num_from_start);
    rewind(file_pointer);
    if(char_num_from_start==-1)
    {
        fclose(file_pointer);
        return;
    }
        copy_str(file_pointer , char_num_from_start , char_num_to_copy , back_or_forward);
        printf("the string was successfully copied.\n");
        fclose(file_pointer);
    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"cutstr")){

    char file_name[100];
    int flag=0 , line_num , char_pos , char_num_to_cut , char_num_from_start;
    char back_or_forward;
    if (check_switch("--file"))
       return;
    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;

    undo_helper1(file_pointer);

    if (check_switch("--pos")) return;

    scanf("%d:%d" , &line_num , &char_pos);
    printf("%d\n%d\n" , line_num , char_pos);
    if (check_switch("-size")) return;

    scanf("%d" , &char_num_to_cut);
    //printf("%d\n" , char_num_to_cut);

    scanf(" -%c" , &back_or_forward);
    //printf("%c\n" , back_or_forward);

    char_num_from_start= reach_to_pos(file_pointer , line_num , char_pos );
    printf("char from start is:%d\n" ,char_num_from_start );
    rewind(file_pointer);
        if(char_num_from_start==-1)
        {
            fclose(file_pointer);
            return;
        }

    cut_str(file_pointer ,file_name , char_num_from_start , char_num_to_cut , back_or_forward);

    printf("the string was successfully cut from the file.\n");
    undo_helper2(file_name);
    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"pastestr")){
    printf("eybaba");
    char file_name[100];
    int flag=0, line_num , char_pos ;
    if (check_switch("--file")) return;

    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;

    undo_helper1(file_pointer);

    if (check_switch("--pos"))
       return;

    scanf("%d:%d" , &line_num , &char_pos);
    int char_num_from_start= reach_to_pos(file_pointer , line_num , char_pos);
    if(char_num_from_start==-1)
    {
        fclose(file_pointer);
        return;
    }
    rewind(file_pointer);
        paste_str(file_pointer , file_name , char_num_from_start);
        printf("the string was successfully pasted in the file.\n");
        undo_helper2(file_name);
    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"find" )){
        char str_to_find[MAXSTRSIZE];
    char switches[100];
    char switch_1[10];
    char switch_2[10];
    char switch_3[10];
    char file_name[100];
    int index_keeper[100];
    int word_keeper[100];
    int size_keeper[100];
    int end_pos[100];
    int flag=0  , index=0;
    int first_switch=-1 , second_switch=-1 ;
    int at_num=0;
    if (check_switch("--str"))
        return;
    // scanning the str to find
    scanf_str(str_to_find);
    int mode= str_interpreter(str_to_find);
    printf("your str is: %s\n" , str_to_find);
    if (check_switch("--file")) return;

    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;
    gets(switches);
    //printf("strlen switches:%d\n" , strlen(switches));

    sscanf(switches , " %s%s%s" , switch_1 , switch_2 , switch_3);
    if(!strcmp(switch_1 , "--at")){
        sscanf(switches , "%s %d%s" , switch_1 , &at_num , switch_2);
        //printf("at_num:%d" , at_num);
    }
    if(!strcmp(switch_2 , "--at")){
        sscanf(switches , "%s%s %d" , switch_1 , switch_2 , &at_num);
        //printf("at_num:%d" , at_num);

    }
    //printf("%s\n%s\n%d\n" , switch_1 , switch_2 , at_num );
    first_switch=switch_interpret(switch_1);
    second_switch=switch_interpret(switch_2);
    //printf("first:%d second:%d\n" , first_switch , second_switch);


        find_str(file_pointer , str_to_find , word_keeper , index_keeper , size_keeper , end_pos ,  first_switch , second_switch , at_num , 0 , mode);
    }
    //----------------------------------------------------------------
    else if(!strcmp(command , "replace")){
        replace();
    }
    //----------------------------------------------------------------
    else if(!strcmp(command ,"grep")){
        int mode=0;
        int jump=0;
        int line_count=0;
        int flag=0;
        char file_name[100];
        char string_to_grep[MAXSTRSIZE];
        char string_holder[MAXSTRSIZE];
        char switch_1[100];
        char files[100];
        scanf("%s" , switch_1);
        if(!strcmp(switch_1 , "-l")){
            if (check_switch("--str")) return;
            mode=1;
        }
        else if(!strcmp(switch_1 , "-c")){
            if (check_switch("--str")) return;
            mode=2;
        }
        else if(!strcmp(switch_1 , "--str")){}
        else
        {
            printf("you have entered the wrong switch");
            return;
        }
        scanf_str(string_to_grep);
        str_interpreter(string_to_grep);

        if (check_switch("--files")) return;
        int i=0;
        getchar();
        gets(files);
        while(1){
            int line_keeper[100]={0};
            char path[100];
            chdir(first_address);
            if(jump>=strlen(files)) break;
            //printf("jump:%d strlen:%d\n" , jump , strlen(files));
            sscanf(files+jump , "%s" , path);
            jump+=strlen(path)+1;
            FILE* file_pointer;
            file_pointer= file_pointer_maker2(path , file_name);
            if(file_pointer==NULL) return;



            int found_line=grep(file_pointer , string_to_grep , line_keeper);
            line_count+=found_line;
           // printf("%d\n" , mode);
            if(mode==0){
                for(int j=0; !feof(file_pointer); j++){
                        fgets(string_holder , 100 , file_pointer);
                    if(line_keeper[j]){
                        printf("%s: " , file_name);
                        puts(string_holder);
                    }
                }

            }
            if(mode==1){
                if(found_line)
                    printf("%s\n" , file_name);
            }
            i++;
        }
        if(mode==2){
            printf("%d\n" , line_count);
        }

    }
    //----------------------------------------------------------------
    else if(!strcmp(command , "undo")){
    char file_name[100];
    int flag=0;
    if (check_switch("--file"))
       return;
    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;

    undo(file_pointer , file_name);
    printf("done!\n");
    }
    //----------------------------------------------------------------
    else if(!strcmp(command , "auto-indent")){
    char file_name[100];
    int flag=0;
    if (check_switch("--file")) return;

    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    if(file_pointer==NULL) return;

    undo_helper1(file_pointer);
    auto_indent(file_pointer , file_name);
    printf("the file was successfully auto-indented\n");
    undo_helper2(file_pointer);

    }
    //----------------------------------------------------------------
    else if(!strcmp(command , "compare")){
    char file_name[100];
    char file_name2[100];
    if (check_switch("--files")) return;
    int flag=0;
    FILE* file_pointer1;
    file_pointer1= file_pointer_maker(file_name);
    if(file_pointer1==NULL) return;

    chdir(first_address);

    FILE* file_pointer2;
    file_pointer2= file_pointer_maker(file_name);
    if(file_pointer2==NULL) return;

    text_comperator(file_pointer1 , file_pointer2);

    }
    //----------------------------------------------------------------
    else if(!strcmp(command , "tree")){
        int depth;
        scanf("%d" , &depth);
        chdir("root");
        if(depth<-1){
            printf("you have entered the wrong depth.\n");
            return;
        }
        if(depth==-1)
            directory_tree(50 , 0);

        else
            directory_tree(depth , 0);
        chdir(first_address);
    }
    //----------------------------------------------------------------
    else if(!strcmp(command , "exit")){
        EXIT=1;
        return;
    }
    else {
        gets(useless);
        printf("you have entered the wrong command.\n");
    }

    chdir(first_address);
}

int reach_to_pos(FILE* file_pointer , int line_number , int char_pos){
    int char_counter=0;
   // printf("line number:%d char_pos:%d" , line_number , char_pos);
    char tmp[100];
    for(int i=0; i< line_number-1; i++){
        fgets(tmp , 100 , file_pointer);
       // printf("%d st line is:%s" , i , tmp);
        if(feof(file_pointer))
        {
            gets(useless);
            printf("this line does not exist in the file.\n");
            return -1;
        }
        char_counter+=strlen(tmp);
    }

    for(int i=0; i<char_pos ;i++){
        char tmp1= fgetc(file_pointer);
        if(feof(file_pointer))
        {
            gets(useless);
            printf("this position does not exist in the file\n");
            return -1;
        }
        char_counter+=1;
    }

return char_counter;
}
int str_interpreter(char * str ){
    char result_str[MAXSIZE];
    int flag=0;
    int i=0;
    int j=0;
    if(str[0]=='*'){
        flag=1;
        i++;
    }
    for(; str[i]!='\0'; i++ , j++){
        if(str[i]=='\\' && str[i+1]=='n'){
            result_str[j]='\n';
            i++;
            continue;
        }
        if(str[i]=='\\' && str[i+1]=='\\'){
            result_str[j]='\\';
            i++;
            continue;
        }
        if(str[i]=='\\' && str[i+1]=='*'){
            result_str[j]='\n';
            i++;
            continue;
        }
        if(str[i]=='*' && str[i+1]=='\0'){
            flag=2;
            result_str[j]='\0';
            i++;
            continue;
        }
            result_str[j]=str[i];
    }
    result_str[j]='\0';
    //printf("your entered str is:/%s/\n and the result is:/%s/\n" , str , result_str);
    strcpy(str , result_str);
    //printf("%s\n%s" , str , result_str);
return flag;
}
void scanf_str(char str[]){
    int flag=0;
    int i;
    getchar();
    char first_char=getchar();
    //printf("your first character is %c\n" , first_char);
    if(first_char== '"')
    {
        str[0]=getchar();
        i=1;
        while(!(str[i-2]!='\\' && str[i-1]=='\"'))
        {
            scanf("%c" , &str[i]);
            i++;
        }
        str[i-1]='\0';
    }

    else
    {
        ungetc(first_char , stdin);
        scanf("%s" , str);
        //printf("first getting:/%s/\n" , str);
    }
        getchar();
}
int directory_existence(const char *path){

    struct stat stats;

    stat(path, &stats);
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}
int check_switch(char* my_switch){
    char switch1[20];
    scanf("%s" , switch1);
    //printf("%s" , switch1);
    if(strcmp(switch1 , my_switch)){
        gets(useless);
        printf("you have entered the wrong switch\ntry %s this time\ninstead of %s" , my_switch , switch1);
    return 1;
    }
    return 0;
}
int switch_interpret(char* a){
    if(!strcmp(a , "--byword"))
        return 0;
    if(!strcmp(a , "--at"))
        return 1;
    if(!strcmp(a , "--all"))
        return 2;
    if(!strcmp(a , "--count"))
        return 3;
    return -1;
}
void file_copy(FILE* dest , FILE* src){
    char tmp[MAXSTRSIZE];

    int i=0;
    while(!feof(src)){
        fgets(tmp ,100 , src);
        fputs(tmp , dest);
        //printf("\nyour transfered str is:%s\n" , tmp);

        i++;
        if(i>10)
            break;
    }
}
int line_counter(FILE* fp){
    int i=0;
    char tmp[MAXSTRSIZE];
    while(!feof(fp)){
        fgets(tmp , 100 , fp);
        i++;
    }
return i;
}
void undo_helper1(FILE* file_pointer){
    getcwd(file_location , 100);
    chdir(undo_address);
    FILE* tmp_fp;
    tmp_fp=fopen("tmp.txt" ,"w");
    file_copy(tmp_fp , file_pointer);
    fclose(tmp_fp);
    rewind(file_pointer);
    chdir(file_location);
}
void undo_helper2(char* file_name){
    char name_container[100];
    chdir(undo_address);
    FILE* tmp_fp;
    tmp_fp=fopen("tmp.txt" ,"r");

    if(!directory_existence(file_name))
        mkdir(file_name);
    chdir(file_name);
    FILE* undo_fp;
    for(int i=0; ;i++){
        f_name_maker(name_container , i);
        undo_fp=fopen(name_container , "r");
        if(undo_fp==NULL) break;
        fclose(undo_fp);
    }
    undo_fp=fopen(name_container , "w");
    //printf("%s" , file_name);
    file_copy(undo_fp , tmp_fp);
    fclose(tmp_fp);
    remove("tmp.txt");
    fclose(undo_fp);
}
void f_name_maker(char* str , int i){
    char tmp[MAXSTRSIZE];
    sprintf(tmp , "%d" , i);
    strcat(tmp , ".txt");
    strcpy(str , tmp);
}
int check_char(char a){
    if(a=='{') return 1;
    if(a=='}') return -1;
    if(a=='\n'|| a==' ' ||a=='\t') return 0;
    else return 2;
}
int white_counter(char* str , int index){
    if(check_char(str[index+1])==0)
        return white_counter(str , index+1)+1;
    else return 0;
}

void create_file(){
    if(check_switch("--file")) return;

    char file_location[100];
    getchar();
    char first_char= getchar();
    //printf("%c" , first_char);
    if(first_char== '/'){
    scanf("%s" , file_location);
    for(int i=0; file_location[i]!='\0'; i++){
        if(file_location[i]==' '){
            printf("you have entered the wrong file location\n");
            return;
        }
    }
    }
    else if(first_char=='"'){
        int i=0;
        while(1){
            scanf("%c" , file_location+i);
            if(*(file_location+i)=='"'){
                *(file_location+i)='\0';
                break;
                }
            i++;
        }
        for(;*(file_location+i-1)!='/';i--){
            if(*(file_location+i-1)==' ')
                if(*(file_location+i-2)=='.')
                    printf("you have entered the wrong file name\n");
        }
        //printf("%s" , file_location);
    }
    else{
        printf("you have entered the wrong file location\n");
        return;
    }
    //gets();
    char tmp[100];
    char* token= strtok(file_location , "/");
    strcpy(tmp , token);
    while (1) {
        //printf("%s\n", token);
        token = strtok(NULL, "/");
        if(token!=NULL){
        if(directory_existence(tmp))
            chdir(tmp);
        else{
            mkdir(tmp);
            chdir(tmp);
        }
        strcpy(tmp ,token);
        }
        else break;
    }
    FILE* file;
    if(fopen(tmp, "r")!=NULL){
        gets(useless);
        printf("this file already exists.\n");
        return;
    }
    file= fopen(tmp, "w");
    fclose(file);
    printf("the file was successfully created\n");

}

void insert_str(FILE* file_pointer , char* file_name , int char_pos , char* string_to_insert){
    char string_holder[100];

    // insertstr --file /root/ali.txt --str "Alireza" --pos 2:0

    rewind(file_pointer);

    FILE * new_file_p;
    new_file_p= fopen("tmp.txt"  , "w");
   // printf("sls\n");
    for(int i=0; i<char_pos; i++){
        char tmp=fgetc(file_pointer);
        fputc(tmp , new_file_p);
    }
    for(int i=0; string_to_insert[i] != '\0'; i++)
    {
        fputc(string_to_insert[i] , new_file_p);
    }

    while(!feof(file_pointer)){
    fgets(string_holder , 100 , file_pointer);
    fputs(string_holder , new_file_p);

    }
    //fputc(NULL , new_file_p);

    fclose(new_file_p);
    fclose(file_pointer);
    remove(file_name);
    rename("tmp.txt", file_name);
}

void cat_str(FILE* file_pointer , char* file_name){
    //rewind(file_pointer);
    char tmp[100];
    char tmp2[100];
    printf("contents of %s:\n---------------------------------------\n" , file_name);
    while(!feof(file_pointer)){
        fgets(tmp , 100 , file_pointer);
        if(!strcmp(tmp , tmp2))
            continue;
        printf("%s" , tmp);
        strcpy(tmp2 , tmp);
    }
    printf("\n---------------------------------------\n");
    fclose(file_pointer);
}

void remove_str(FILE* file_pointer ,char* file_name , int char_num_from_start , int char_num_to_remove , char back_or_forward){
    // removestr --file /root/ali.txt --pos 2:0 -size 1 -f
    rewind(file_pointer);
    char string_holder[100];
    FILE * new_file_p;
    new_file_p= fopen("tmp.txt"  , "w");
    //printf("the char position is:%d\n" , char_num_from_start);
    if(back_or_forward=='f')
    {
        for(int i=0; i<char_num_from_start ;i++){
        char tmp1= fgetc(file_pointer);
        fputc(tmp1 , new_file_p);
        }
        for(int i=0; i<char_num_to_remove && !feof(file_pointer) ; i++)
        {
            fgetc(file_pointer);
        }
        while(!feof(file_pointer))
        {
            fgets(string_holder , 100 , file_pointer);
            fputs(string_holder , new_file_p);
        }
    }

    else if(back_or_forward=='b')
    {

        int remainer= char_num_from_start-char_num_to_remove;
        if(remainer<0)
        {
            for(int i=0; i<char_num_from_start; i++)
            {
                fgetc(file_pointer);
            }
        }
        else
        {
            for(int i=0; i<remainer; i++)
            {
                char tmp= fgetc(file_pointer);
                fputc(tmp , new_file_p);
            }

            for(int i=0; i<char_num_to_remove; i++)
            {
                fgetc(file_pointer);
            }
        }
        while(!feof(file_pointer))
        {
            fgets(string_holder , 100 ,file_pointer);
            fputs(string_holder , new_file_p);
        }

    }
        fclose(file_pointer);
        remove(file_name);
        fclose(new_file_p);
        rename("tmp.txt" , file_name);
       // removestr --file /root/ali/yes.txt --pos 1:8 -size 4 -b
    }

void copy_str(FILE* file_pointer , int char_num_from_start , int char_num_to_copy  ,char back_or_forward){
    // copystr --file /root/ali.txt --pos 2:0 -size 10 -f
    char tmp_str[100];
    if(back_or_forward=='f')
    {
        for(int i=0; i<char_num_from_start; i++){
            fgetc(file_pointer);
        }
        int i=0;
        for(; i<char_num_to_copy && !feof(file_pointer); i++)
        {
            tmp_str[i]= fgetc(file_pointer);
        }
        tmp_str[i]='\0';

    }
    else if(back_or_forward=='b')
    {

        int remainer= char_num_from_start-char_num_to_copy;
        if(remainer<0)
        {
            int i=0;
            for(; i<char_num_from_start; i++)
            {
                tmp_str[i]= fgetc(file_pointer);
                printf("%c" , tmp_str[i]);
            }
            tmp_str[i]='\0';
            printf("\ndone\n");
        }

        else
        {
            for(int i=0; i<remainer; i++)
            {
                fgetc(file_pointer);
            }
            int i=0;
            for(; i<char_num_to_copy; i++)
            {
                tmp_str[i]= fgetc(file_pointer);
                printf("%c" , tmp_str[i]);
            }
            tmp_str[i]='\0';
        }
         printf("\ndone\n");
    }
    strcpy(clipBoard , tmp_str);
    printf("you str is%s\n" , clipBoard);

}

void cut_str(FILE* file_pointer, char* file_name, int char_num_from_start , int char_num_to_cut  , char back_or_forward){
    //printf("previous one:%s\n" , clipBoard);
    copy_str(file_pointer , char_num_from_start , char_num_to_cut , back_or_forward);
    rewind(file_pointer);
    remove_str(file_pointer , file_name , char_num_from_start , char_num_to_cut ,back_or_forward);
// cutstr --file /root/ali.txt --pos 2:0 -size 10 -f
}

void paste_str(FILE * file_pointer , char* file_name ,int char_num_from_start){
  //  pastestr --file /root/ali.txt --pos 2:0
    insert_str(file_pointer , file_name , char_num_from_start , clipBoard);

}

void find_printer(FILE* file_pointer , int* index_keeper , int* word_keeper  ,int counter, int first_switch , int second_switch , int at_num ){

    int cross= first_switch* second_switch;
    int plus= first_switch+ second_switch;
    if(first_switch<0 && second_switch<0)
    {
        if(counter>0)
            printf("%d\n" , index_keeper[0]);
        else
            printf("-1\n");
    }
    else if(cross>0 || first_switch==second_switch)
    {
        printf("you have entered the wrong set of switches");
        return;
    }
    else if(cross==0 && plus<0)
    {
        if(counter>0)
            printf("%d\n" , word_keeper[0]);
        else
            printf("-1\n");
    }
    else if(cross<0)
    {
        if(plus==0){
            if(at_num<=counter)
            printf("%d\n" , index_keeper[at_num-1]);
            else
                printf("-1\n");
        }
        if(plus==1){
            if(counter>0){
                for(int i=0; i<counter; i++){
                    printf("%d " , index_keeper[i]);
                }
                printf("\n");
            }
            else
                printf("-1\n");
        }
        if(plus==2){
            printf("%d\n" , counter);
        }
    }
    else
    {
        if(plus==1){
            if(at_num<=counter)
                printf("%d\n" , word_keeper[at_num-1]);

            else
                printf("-1\n");
        }
        if(plus==2){
            if(counter>0){
                for(int i=0; i<counter; i++){
                    printf("%d " , word_keeper[i]);
                }
                printf("\n");
            }
            else
                printf("-1\n");
        }
        if(plus==3){
            printf("%d" , counter);
        }
    }
}

int find_str(FILE* file_pointer , char* string_to_find  , int *word_keeper , int* index_keeper  , int* size_keeper , int* end_pos ,int  first_switch , int second_switch  , int at_num , int replace_mode , int mode){
    char space_finder;
    int word_num=1;
    int index_num=0;
    int flag=0;
    int last_space=-1;
    int next_space;
    int counter=0;
    int index=0;
    for(int i=0 ; !feof(file_pointer) ; i++ , index++)
    {
        fseek(file_pointer , i, SEEK_SET);
        space_finder= fgetc(file_pointer);
       // printf("%c\n" , space_finder);
        if(space_finder==' ' || space_finder=='\n'){
            word_num++;
            //printf("wordnum:%d\n" , word_num);
            last_space=index;
        }
        if(space_finder=='\n'){
            //word_num++;
           // printf("wordnum:%d\n" , word_num);
            i++;
            last_space=index;

        }

        fseek(file_pointer , -1, SEEK_CUR);

        int j=0;
        for(; j<strlen(string_to_find)&& !feof(file_pointer) ; j++){
            char tmp= fgetc(file_pointer);
            //printf("%c " , tmp);
            if(tmp!= string_to_find[j]){
                break;
                //printf("\n");
            }
        }

        if(j==strlen(string_to_find))
        {
            flag=1;
            if(mode==1){ ////  *a
                index_keeper[counter]=last_space+1;
                size_keeper[counter]=index+j-last_space-1;
            }
            else if(mode==2){ ////  a*
                index_keeper[counter]=index;

                int k=0;
                for(; space_finder!=' ' && space_finder!='\n' &&space_finder!=EOF ; k++){
                    space_finder=getc(file_pointer);
                }
                printf("tonextspace:%d\n"  , k);
                size_keeper[counter]=j+k-1;
                end_pos[counter]=i+j+k;
            }
            else{
                index_keeper[counter]=index;
                size_keeper[counter]=j;
               // printf("size is:%d\n" , j);
            }
            word_keeper[counter]=word_num;

            counter++;
        }

    }

    rewind(file_pointer);
    if(replace_mode==0){
    find_printer(file_pointer , index_keeper , word_keeper , counter , first_switch , second_switch , at_num);
    }
    return counter;
// find --str hello --file /root/a.txt --byword
//find --str " world" --file /root/a.txt --byword
}

void replace(){
    char str_to_replace[100];
    char str_to_insert[100];
    char switches[100];
    char switch_1[100];
    char file_name[100];
    int word_keeper[100];
    int index_keeper[100];
    int size_keeper[100];
    int end_pos[100];
    int flag=0  , index=0;
    int switch_num=-1;
    int at_num;
    if (check_switch("--str1"))
        return;
    scanf_str(str_to_replace);
    int mode= str_interpreter(str_to_replace);
    printf("your str is: %s\n" , str_to_replace);
    if (check_switch("--str2"))
        return;
    scanf_str(str_to_insert);
    str_interpreter(str_to_insert);
    if (check_switch("--file"))
        return;
    FILE* file_pointer;
    file_pointer= file_pointer_maker(file_name);
    undo_helper1(file_pointer);

    if(file_pointer==NULL) return;
    gets(switches);
    if(strlen(switches)>1){
        sscanf(switches , " %s" , switch_1);
        if(!strcmp(switch_1 , "--at")){
            sscanf(switches , "%s %d" , switch_1 , &at_num);
            switch_num=1;
        }

        else if(!strcmp(switch_1 , "--all")){
            switch_num=2;
        }
        else
        {
            printf("you have entered the wrong switch\n try --str or --all this time.");
            return;
        }
            printf("you switch is:%d" ,switch_num);

    }    printf("mode:%d\n" , mode);

    int number_found= find_str(file_pointer , str_to_replace , word_keeper , index_keeper, size_keeper , end_pos , switch_num , -1 , at_num ,  1  ,mode);
    //printf("number found:%d\nstr to replace:%s\n work keeper:%d\nsize:%d" , number_found , str_to_insert , word_keeper[0] , size_keeper[0]);
    if(!number_found){
        printf("the string %s was not found in the file\n" , str_to_replace);
        return;
    }

    if(switch_num==-1){
        remove_str(file_pointer , file_name , index_keeper[0] , size_keeper[0] , 'f');
        file_pointer=fopen(file_name , "r");
        insert_str(file_pointer , file_name , index_keeper[0] , str_to_insert);
    }
    if(switch_num==1){
        if(at_num>number_found){
            printf("we could not find %d copies of the string %s in the file.\n" , at_num , str_to_replace);
        }
        remove_str(file_pointer , file_name , index_keeper[at_num-1] , size_keeper[at_num-1], 'f');
        file_pointer=fopen(file_name , "r");
        insert_str(file_pointer , file_name ,index_keeper[at_num-1] , str_to_insert);
    }
    if(switch_num==2){
        for(int i=number_found-1; i>=0 ; i--){
            if(mode==1 &&index_keeper[i]==index_keeper[i+1]) continue;
            if(mode==2 && end_pos[i]==end_pos[i-1]) continue;
            //printf("%d:index:%d\nsize:%d\n"  , i, index_keeper[i] , size_keeper[i] );
            remove_str(file_pointer ,file_name , index_keeper[i] , size_keeper[i], 'f');
            file_pointer=fopen(file_name , "r");
            insert_str(file_pointer ,file_name , index_keeper[i] , str_to_insert);
            file_pointer=fopen(file_name , "r");
        }
    }
    printf("the strings have been successfully replaced.\n");
        undo_helper2(file_name);
        //replace --str1 "ali" --str2 "ali ?" --file /root/a.txt --all
}

int grep(FILE* file_pointer , char * string_to_find , int *line_keeper){
    //grep --str hello --files /root/ali.txt /root/a.txt
    //grep -c --str hello --files /root/ali.txt /root/a.txt
    int line_num=0;
    int counter=0;
    for(int i=0; !feof(file_pointer) ; i++)
    {
        fseek(file_pointer , i, SEEK_SET);
        char newline_finder= fgetc(file_pointer);
        //printf("char:%c\n" , newline_finder);
        if(newline_finder=='\n'){
            i++;
            line_num++;
            continue;
        }
        fseek(file_pointer , -1, SEEK_CUR);
        int j=0;
        for(; j<strlen(string_to_find)&& !feof(file_pointer) ; j++){
            char tmp= fgetc(file_pointer);
            //printf("%c " , tmp);
            if(tmp!= string_to_find[j]){
                break;
                //printf("\n");
            }
        }

        if(j==strlen(string_to_find))
        {
           // printf("linenum:%d\n" , line_num);
            line_keeper[line_num]=1;
           // printf("\n%d\nsucsess\n" , i);
            counter++;
            i+strlen(string_to_find)-1;
        }
    }
    rewind(file_pointer);
return counter;
}

void undo(FILE* file_pointer , char * file_name){
    char name_container[100];
    char real_name[100];
    getcwd(file_location , 100);

    fclose(file_pointer);
    file_pointer=fopen(file_name , "w");

    chdir(undo_address);
    chdir(file_name);
    FILE* undo_fp;
    for(int i=0; ; i++){
        f_name_maker(name_container , i);
        undo_fp=fopen(name_container , "r");
        if(undo_fp==NULL) break;
        fclose(undo_fp);
        strcpy(real_name , name_container);
    }
    undo_fp= fopen(real_name , "r");

    file_copy(file_pointer , undo_fp);
    fclose(undo_fp);
    remove(real_name);
    fclose(file_pointer);
    chdir(file_location);

}

void auto_indent(FILE* file_pointer , char * file_name){
    char file[100];
    char res_file[100];
    int what_char;
    int last_char=0;
    int white_num;
    int i=0;
    int open_brace=0;
    for(;!feof(file_pointer); i++){
        file[i]= fgetc(file_pointer);
    }
    file[i]='\0';
    printf("%s" , file);
    int j=0;
    for(int i=0 ; file[i]!= '\0'; i++ , j++){
        what_char= check_char(file[i]);
        if(what_char==0){
            white_num=white_counter(file , i);
            if(last_char==2){
                res_file[j]=' ';
            }
            else j--;

            i+=white_num;
            //printf("white number is:%d\n" , white_num);
        }
        else if(what_char==1|| what_char== -1){
            if(last_char==2){
                res_file[j]=' ';
                j++;
                res_file[j]=file[i];
                //printf("%d:%c\n" , i , res_file[j]);
            }
            else {
                res_file[j]=file[i];
            }
        }
        else{
            res_file[j]=file[i];
           // printf("%d:%c\n" , i , res_file[j]);
        }
    last_char=what_char;
    }
    res_file[j]='\0';
    printf("%s" , res_file);


    fclose(file_pointer);
    file_pointer=fopen(file_name , "w");


    for(int i=0; res_file[i]!='\0'; i++){
        what_char=check_char(res_file[i]);
        if(what_char==1){
            open_brace+=what_char;
            if(last_char==-1 || last_char== 1){
            fprintf(file_pointer , "\n");
            printf("hasti ya na");
            for(int index=0; index<open_brace-1; index++){
                fprintf(file_pointer , "    ");
            }
            fputc(res_file[i] , file_pointer);
            }
            else putc(res_file[i] , file_pointer);        /*    if(check_char(res_file[i+1])=='}'){
                for(int index=0; index<open_brace-1; index++){
                fprintf(file_pointer , "    ");
                }
                fputc('}' , file_pointer);
                i++;
            }
            else{
                for(int index=0; index<open_brace; index++){
                fprintf(file_pointer , "    ");
                }
            }*/
        }
        else if(what_char==-1){
            open_brace+=what_char;
            fprintf(file_pointer , "\n");
            for(int index=0; index<open_brace; index++){
                fprintf(file_pointer , "    ");
            }
            fputc('}' , file_pointer);
            //if(res_file[i+1]!='}'){
            //   fputc('\n' , file_pointer);
            //}
        }
        else if(what_char==2){
            if(last_char==-1 || last_char== 1){
            fprintf(file_pointer , "\n");
            for(int index=0; index<open_brace; index++){
                fprintf(file_pointer , "    ");
            }
            fputc(res_file[i] , file_pointer);
            }
            else putc(res_file[i] , file_pointer);
        }
        else putc(res_file[i] , file_pointer);
        last_char=what_char;

    }
    fclose(file_pointer);
    //fputc('\0' , file_name);
    //cat_str(file_pointer , file_name);
    printf("the file was auto-indented successfully.");
    // auto-indent --file /root/a.txt
}

void text_comperator(FILE * fp1 , FILE * fp2){
    char line1[100];
    char line2[100];
    char word1[20];
    char word2[20];
    int endofone=0 , jump1 , jump2 , startpos , length_1 , length_2;
    int end1;
    int end2;
    int diff_word;
    int differ=0;
    chdir(first_address);
    FILE* output_fp;
    output_fp= fopen("output.txt" , "w");

    int line_num1=line_counter(fp1);
    int line_num2=line_counter(fp2);
    rewind(fp1);
    rewind(fp2);
    printf("linenum1:%d\n linenum2:%d\n" , line_num1 , line_num2);

    if(line_num1==line_num2){
        endofone=0;
        end1=line_num1;
        end2=line_num2;

    }
    else if(line_num1>line_num2){
        endofone=2;
        end1=line_num2;
        end2=line_num1;
    }
    else{
        endofone=1;
        end1=line_num1;
        end2=line_num2;
    }

    for(int i=0; i<end1 ; i++){
        fgets(line1 , 100 , fp1);
        printf("%s\nlen:%d\n" , line1 , strlen(line1));
        fgets(line2 , 100 , fp2);
        printf("%s\nlen:%d\n" , line2 , strlen(line2));
        differ=0;
        jump1=0;
        jump2=0;
        for(int j=0; ; j++){
            sscanf(line1+jump1 , "%s" , word1);
            printf("word1:%s\n" , word1);
            sscanf(line2+jump2 , "%s" , word2);
            printf("word2:%s\n" , word2);

            if(strcmp(word1 , word2)){
                differ++;
                if(differ>1)
                    break;
                startpos=jump1;
                length_1=strlen(word1);
                length_2=strlen(word2);
            }
            jump1+=strlen(word1)+1;
            printf("jump1:%d\n" , jump1);
            jump2+=strlen(word2)+1;
            printf("jump2:%d\n" , jump2);

            if(jump1>=strlen(line1) && jump2>=strlen(line2))
                break;
        }
        printf("differ %d th line:%d\n" , i , differ);
        if(differ==0){
            continue;
        }
        if(differ==1){

            fprintf(output_fp , "============ %d ============\n" , i+1);
            for(int i=0; i<startpos; i++){
                fputc(line1[i] , output_fp);
            }
            fprintf(output_fp , ">>");
            for(int i=startpos; i<startpos+length_1; i++){
                fputc(line1[i] , output_fp);
            }
            fprintf(output_fp , "<<");
            fputs(line1+startpos+length_1  , output_fp );



            for(int i=0; i<startpos; i++){
                fputc(line2[i] , output_fp);
            }
            fprintf(output_fp , ">>");
            for(int i=startpos; i<startpos+length_2 ;i++){
                fputc(line2[i] , output_fp);
            }
            fprintf(output_fp , "<<");
                fputs(line2+startpos+length_2  , output_fp );
                fputc(' '  , output_fp);
        }


        if(differ>1){
            fprintf(output_fp , "============ %d ============\n%s\n%s\n" ,i+1 , line1 , line2 );
        }

    }
        if(endofone!=0){
            fprintf(output_fp , "%d" , endofone);
            fprintf(output_fp , ">>>>>>>>>>>>> %d - %d >>>>>>>>>>>>>\n" , end1 , end2);
        }
        if(endofone==1){
            for(int i=end1; i<end2 ; i++){
                fgets(line2 , 100 , fp2);
                fputs(line2 , output_fp);
            }
        }
        if(endofone==2){
            for(int i=end1; i<end2 ; i++ ){
                fgets(line1 , 100 , fp1);
                fputs(line1 , output_fp);
            }
        }
    fclose(output_fp);
    output_fp= fopen("output.txt" , "r");
    cat_str(output_fp , "output.txt");
    //compare --files /root/ali.txt /root/a.txt
}

void directory_tree(int depth ,int print_helper){
    if(depth==-1) return;
    //if(print_helper==0) printf("root\n");
    struct dirent* files_in_here;
    DIR* dir= opendir(".");
    if(dir==NULL) return;
    while((files_in_here= readdir(dir))!=NULL){
        if(!strcmp(files_in_here->d_name , ".")|| !strcmp(files_in_here->d_name , "..")) continue;

        for(int i=0; i<print_helper; i++){
            printf("|   ");
        }
        printf("|---");

        printf("%s\n" , files_in_here->d_name);
        if(chdir(files_in_here->d_name)==0){

            directory_tree(depth-1 , print_helper+1);
            chdir("..");
        }
    }
    closedir(dir);
}



