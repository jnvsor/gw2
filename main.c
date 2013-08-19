/*
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <string.h>
#include <regex.h>

typedef struct {
  char * name;
  int score;
  char * date;
} guildie;

static inline int type(xmlNodePtr cur, char * str){
  return !xmlStrcmp(cur->name,(const xmlChar *) str);
}

int class(xmlNodePtr cur, char * str){
  char * classes;
  char * class;
  
  classes = (char *) xmlGetProp(cur,(xmlChar *) "class");
  class = strtok(classes," ");
  
  /* 
   * Note that strtok_r() from posix isn't neccesary as
   * the only function that uses strtok (This one) now
   * loops over the whole list of classes at once instead
   * of nested while loops going one at a time. Less
   * efficient, way cleaner. And it should compile on win32
   * too now.
   */

  while(class != NULL){
    if(!strcmp(class,str)){
      xmlFree(classes);
      return 1;
    }
    class = strtok(NULL," ");
  }
  xmlFree(classes);
  return 0;
}

int get_score(xmlNodePtr cur,xmlDocPtr doc){
  char * holder;
  int value;
  
  while(cur != NULL){
    if(type(cur,"span") && class(cur,"cell-inner")){
      holder = (char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      value = atoi(holder);
      xmlFree(holder);
      return value;
    }
    cur = cur->next;
  }
  return -1;
}

char * get_date(xmlNodePtr cur, xmlDocPtr doc){
  while(cur != NULL){
    if(class(cur,"additional")){
      return (char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
    }
    cur = cur->next;
  }
  return NULL;
}

guildie get_guildie(xmlNodePtr cur,xmlDocPtr doc){
  guildie g;
  g.name = NULL;
  g.date = NULL;
  g.score = 0;
  
  if(!type(cur,"tr")){
    return g;
  }
  
  xmlNodePtr span;
  
  cur = cur->xmlChildrenNode;
  while(cur != NULL){
  
    if(type(cur,"td")){
    
      if(class(cur,"name")){
        g.name = (char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      }
      else if(class(cur,"achievements")){
        span = cur->xmlChildrenNode;
        
        while(span != NULL){
          if(type(span,"span") && class(span,"hasAdditional")){
            g.score = get_score(span->xmlChildrenNode,doc);
            g.date = get_date(span->xmlChildrenNode,doc);
            break;
          }
          
          span = span->next;
        }
      }
    }
    cur = cur->next;
  }
  return g;
}

void print_csv_data(xmlNodePtr cur,xmlDocPtr doc, regex_t datereg, regex_t trimreg){
  
  guildie g;
  cur = cur->xmlChildrenNode;
  int reti;
  regmatch_t datematch[4];
  regmatch_t trimmatch[1];
  
	printf("Account,Achievement Points,Day,Month,Year\n");
  
  while(cur != NULL){
    g = get_guildie(cur,doc);
    if(g.name != NULL){
    
      reti = regexec(&datereg,g.date,(size_t)4,datematch,0);
      reti += regexec(&trimreg,g.name,(size_t)1,trimmatch,0);
      if(!reti){
        
        g.date[datematch[1].rm_eo] = 0; // Set end of matches to 0
        g.date[datematch[2].rm_eo] = 0;
        g.date[datematch[3].rm_eo] = 0;
        g.name[trimmatch[0].rm_eo] = 0;
        
        printf("%s,%d,%s,%s,%s\n",
          &g.name[trimmatch[0].rm_so],
          g.score,
          &g.date[datematch[2].rm_so],
          &g.date[datematch[1].rm_so],
          &g.date[datematch[3].rm_so]);
      }
      
      xmlFree(g.name);
      if(g.date != NULL){
        xmlFree(g.date);
      }
    }
    cur = cur->next;
  }
}


int main(int argc, char * argv[]){
  
  if(argc < 2){
    printf("Missing file\n");
    return 1;
  }
  
  xmlDocPtr doc = xmlParseFile(argv[1]);
  if (doc == NULL){
    fprintf(stderr,"Document not parsed successfully.\n");
    return 1;
  }
  
  xmlNodePtr cur = xmlDocGetRootElement(doc);
  if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return 1;
	}
	
	if(xmlStrcmp(cur->name, (const xmlChar *) "tbody")){
	  fprintf(stderr,"Wrong root element, expected tbody. Got %s\n",cur->name);
	  return 1;
	}
	
  regex_t datereg,trimreg;
	int reti = regcomp(&datereg, "([0-9]+)/([0-9]+)/([0-9]+)", REG_EXTENDED);
	reti += regcomp(&trimreg, "[^[:space:]].+\\.[0-9]{4}", REG_EXTENDED);
	if(reti){
	  fprintf(stderr,"Couldn't compile regex\n");
	  return 1;
	}
	
	print_csv_data(cur,doc,datereg,trimreg);
  
	xmlFreeDoc(doc);
  return 0;
}
