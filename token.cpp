#include<string>
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<map>
#include<iomanip>
#include<locale.h>
// check git hub

//sure

using namespace std;

#define OFF 0
#define ON 1
#define SENTENCE_MAX 100


map<wstring,int> table;
wstring stmp;

void data_to_sentence(FILE *);
void parse(wchar_t*,wchar_t*);
wchar_t* mystrtok(wchar_t*,const wchar_t*);
wchar_t* convert(const char*);


int main()
{
    FILE *fptr = fopen("./ettoday.rec","r");
    setlocale(LC_ALL,"");
    FILE* fp = fopen("test.txt","w+");
    //wchar_t* ptr;
    //wchar_t sptr[] = L"我是大帥個：我叫劉昌澈，我是大帥個｜阿澈新聞｜我好飽。我叫劉昌澈，謝謝，哏哏，我叫劉昌澈，我是大帥個\n我好飽";
    //wchar_t delimit[] = L"，。：？｜ \n";
    char mytmp[SENTENCE_MAX] = {};

    if( fptr == NULL) {
        fprintf(stdout,"FATA ERROR: unable to open file\n");
        exit(EXIT_FAILURE);
    }

    //parse(sptr,delimit);
    //start to parse
    data_to_sentence(fptr);

    for(map<wstring,int>::iterator recp = table.begin();recp!=table.end();recp++)
    {
        fwprintf(fp,L"%ls %d\n",(recp->first).c_str(),recp->second);

    }
    cerr<<table.size()<<endl;
    
    return 0;
}


void data_to_sentence(FILE* fptr)
{
    char *tmp = NULL;
    char *ptr = NULL; //point to tmp since I cannot modify tmp
    char *header;
    char *head_delim = ":";
    int i = 0;
    int a = 0;
    size_t len = 0;
    int flag = OFF;
    wchar_t delimit[] = L"，。：？｜\n";


    while((a = getline(&tmp,&len,fptr) )!=-1)
    {
    //    if(i==50) break;
        //fprintf(stderr,"%s",tmp);
        if(flag==ON && strcmp(tmp,"@\n")==0) 
        {
            flag = OFF; //reach the end of body --> no need to parse sentence
            continue;
        }

        if(flag == OFF) header = strtok(tmp,head_delim);

        if( header!=NULL && strcmp(header,"@body")==0)
        {
            flag = ON;
            ptr = tmp;
            //remove string @body
            ptr+=6;
            //convert char to wchar_t
            wchar_t  ws[100];
            swprintf(ws, 100, L"%hs", ptr);
            // start to parse sentence
            parse(ws,delimit);

            header = NULL;
        }

        else if (header==NULL && flag == ON) //mean still in body
        {
            //continue to cut word
            ptr = tmp;
            //convert char to wchar_t
            wchar_t  ws[100];
            swprintf(ws, 100, L"%hs", ptr);
            // start to parse sentence
            parse(ws,delimit);
            //fprintf(stderr,"------still parsing-------\n");
        }
        i++;
    
    }
}



wchar_t* mystrtok(wchar_t *s1,const wchar_t *delmit) //cut sentence and put it into hashtable
{
    static wchar_t* lasttoken = NULL;
    wchar_t *tar = NULL;
    wchar_t tmp[SENTENCE_MAX] = {};
    map<wchar_t*,int>::iterator recp;

    if(s1==NULL)
    {
        s1 = lasttoken;
        if(s1==NULL) return NULL;
    }

    tar = wcspbrk(s1,delmit);

    if(tar)
    {           
        if(*tar == '\n') *tar = ' ';
        wcsncpy(tmp,s1,(tar-s1)+1);
        stmp = wstring(tmp);
        lasttoken = tar+1;
    }

    else
    {   
        wcscpy(tmp,s1);
        stmp = wstring(tmp);
        lasttoken = NULL;
    }

    return s1;

}

void parse(wchar_t *ptmp,wchar_t *delmit)
{
    wchar_t *ptr = mystrtok(ptmp,delmit);
    map<wstring,int> :: iterator recp = table.find(stmp);
    if(recp == table.end())
        table.insert(pair<wstring,int>(stmp,1));
    else recp->second +=1;

    stmp.clear();
    while(ptr!=NULL)
    {
        ptr = mystrtok(NULL,delmit);
        if(ptr!=NULL) //otherwise null will be inserted
        {
            recp = table.find(stmp);
            if(recp == table.end())
                table.insert(pair<wstring,int>(stmp,1));
            else recp->second +=1;
            stmp.clear();
        }
    }
}
