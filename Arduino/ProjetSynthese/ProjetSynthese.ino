#define MAXNODE 100
typedef struct cell{
  uint8_t *times;
  uint8_t numNode;
  struct node* next;
  
}Graph;

Graph graph[MAXNODE];
Graph *actualNode;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  /*flag that define state of the robot*/
  uint8_t flag = 0;
  /*Get the actual way define by angle : 0=South,1:East,2:North,3:West*/
  uint8_t pos;
  /* TODO : actualTime*/
  uint8_t actualTime;


  /*
   * if flag = 0 cars goes from node A to B
   */
  if(flag == 0){

    /*Si un des capteurs détecte le vide alors
     * le flag va en état 2
     */
    if(){
      flag = 1;
    }
  }
  /*
   * if flag = 1 detect node
   */
  if(flag == 1){
    actualNode->times[pos] = times;
    actualNode->next = createNode();
    actualNode = &(actualNode->next);
    //TODO
    if(actualNode->next == NULL || /*TODO:si serveur dit de faire*/){
      flag = 2;
    }
    flag = 0;
  }
  /*
   * if flag = 2 cars go to node already explore
   */
  if(flag == 2){
    
  }
  /*
   * Go on non exploration mode
   */
  if(flag == 3){
    
  }

}
