
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std ;




// classs for each node
class Node
{
public:
   Node(int row, int column, int status)
   {
      this->row = row ;
      this->column = column ;
      this->status = status ;
   }
   
   
   int row, column, status ; // 0:uncleaned, 2:cleaned, 3:R
   vector<Node*> adjacentNodes ;
   vector<Node*> ups ;
   vector<Node*> downs ;
   vector<Node*> backs ;
   int backSteps ;
   int visitedTimes ;
};




int main(int argc, const char * argv[])
{
   int rows, columns, battery, chargerRow, chargerColumn, totalZeros = 0, totalSteps = 0 ;
   int **floorOriginal={}, **floorMapping={} ;
   
   
   if (argc == 2)
   {
      //fileInput(argv[1], floorOriginal, rows, columns, battery, chargerRow, chargerColumn) ;
      
      
      char temp ;

      ifstream infile(argv[1]) ;
      
      if (!infile)
      {
         cout << "fail to open file" ;
      }
      
      infile >> rows >> columns >> battery ;
      infile.get() ;
      
      
      floorOriginal = new int*[rows] ;

      for (int i = 0; i < rows ; i++)
      {
         floorOriginal[i] = new int[columns] ;
         for (int j = 0; j < columns; j++)
         {
            infile >> temp ;
            if (temp == '0')
               floorOriginal[i][j] = 0 ;

            if (temp == '1')
               floorOriginal[i][j] = 1 ;
            
            if (temp == 'R')
            {
               floorOriginal[i][j] = 3 ;
               chargerRow = i ;
               chargerColumn = j ;
            }
         }
         infile.get() ;
      }
   }
   else
      return 1 ;
   
   
   //array for mapping floor
   floorMapping = new int *[rows] ;
   
   for (int i = 0; i < rows; i++)
   {
      floorMapping[i] = new int[columns] ;
      
      for (int j = 0; j < columns; j++)
         floorMapping[i][j] = floorOriginal[i][j] ;
   }
   
   
   

   //array for floor nodes
   Node ***floorNodes ;
   
   floorNodes = new Node**[rows] ;
   for (int i = 0; i < rows; i++)
   {
      floorNodes[i] = new Node*[columns] ;
      for (int j = 0; j < columns; j++)
      {
         if (floorOriginal[i][j] != 1)
            floorNodes[i][j] = new Node(i, j, floorOriginal[i][j]) ;
         else
            floorNodes[i][j] = NULL ;
      }
   }
   
   
   
   
   // root
   Node *root ;
   root = floorNodes[chargerRow][chargerColumn] ;
   root->status = 0 ;
   floorMapping[chargerRow][chargerColumn] = 4 ;

   

   //map nodes
   queue<Node*> expendQueue ;
   expendQueue.push(root) ;
   

   
   // build map
   Node *current, *next ;
   while (!expendQueue.empty())
   {
      current = expendQueue.front() ;
      
      //check for the left node
      if (current->column - 1 >= 0)
      {
         next = floorNodes[current->row][current->column - 1] ;

         if (floorMapping[current->row][current->column - 1] == 4)
         {
            if(find(current->ups.begin(), current->ups.end(), next) == current->ups.end())
               current->downs.push_back(next) ;
            if(find(next->downs.begin(), next->downs.end(), current) == next->downs.end())
               next->ups.push_back(current) ;
            
            // should consider 2 ways
            if (next->backSteps > current->backSteps + 1)
            {
               next->backSteps = current->backSteps + 1 ;
               next->backs.clear() ;
               next->backs.push_back(current) ;
            }
            
            if (next->backSteps == current->backSteps + 1)
               next->backs.push_back(current) ;
            
            
            if(find(current->adjacentNodes.begin(), current->adjacentNodes.end(), next) == current->adjacentNodes.end())
               current->adjacentNodes.push_back(next) ;
            
            if(find(next->adjacentNodes.begin(), next->adjacentNodes.end(), current) == next->adjacentNodes.end())
               next->adjacentNodes.push_back(current) ;
         }
         
         
         if (floorMapping[current->row][current->column - 1] == 0)
         {
            (current->downs).push_back(next) ;
            (next->ups).push_back(current) ;
            (next->backs).push_back(current) ;
            (next->backSteps) = current->backSteps + 1 ;
            (current->adjacentNodes).push_back(next) ;
            (next->adjacentNodes).push_back(current) ;

            // flag for done mapping
            floorMapping[current->row][current->column - 1] = 4 ;
            //push to queue for leter check for next node
            expendQueue.push(next) ;
         }
      }
      
      
      //check for the up node
      if (current->row - 1 >= 0)
      {
         next = floorNodes[current->row - 1][current->column] ;
         
         if (floorMapping[current->row - 1][current->column] == 4)
         {
            if(find(current->ups.begin(), current->ups.end(), next) == current->ups.end())
               current->downs.push_back(next) ;
            if(find(next->downs.begin(), next->downs.end(), current) == next->downs.end())
               next->ups.push_back(current) ;
            
            // should consider 2 ways
            if (next->backSteps > current->backSteps + 1)
            {
               next->backSteps = current->backSteps + 1 ;
               next->backs.clear() ;
               next->backs.push_back(current) ;
            }
            
            if (next->backSteps == current->backSteps + 1)
               next->backs.push_back(current) ;
            
            if(find(current->adjacentNodes.begin(), current->adjacentNodes.end(), next) == current->adjacentNodes.end())
               current->adjacentNodes.push_back(next) ;
            
            if(find(next->adjacentNodes.begin(), next->adjacentNodes.end(), current) == next->adjacentNodes.end())
               next->adjacentNodes.push_back(current) ;
         }
         
         
         if (floorMapping[current->row - 1][current->column] == 0)
         {
            (current->downs).push_back(next) ;
            (next->ups).push_back(current) ;
            (next->backs).push_back(current) ;
            (next->backSteps) = current->backSteps + 1 ;
            (current->adjacentNodes).push_back(next) ;
            (next->adjacentNodes).push_back(current) ;

            // flag for done mapping
            floorMapping[current->row - 1][current->column] = 4 ;
            //push to queue for leter check for next node
            expendQueue.push(next) ;
         }
      }
      
      
      
      //check for the right node
      if (current->column + 1 < columns)
      {
         next = floorNodes[current->row][current->column + 1] ;

         
         if (floorMapping[current->row][current->column + 1] == 4)
         {
            if(find(current->ups.begin(), current->ups.end(), next) == current->ups.end())
               current->downs.push_back(next) ;
            if(find(next->downs.begin(), next->downs.end(), current) == next->downs.end())
               next->ups.push_back(current) ;
            
            // should consider 2 ways
            if (next->backSteps > current->backSteps + 1)
            {
               next->backSteps = current->backSteps + 1 ;
               next->backs.clear() ;
               next->backs.push_back(current) ;
            }
            
            if (next->backSteps == current->backSteps + 1)
               next->backs.push_back(current) ;
            
            
            if(find(current->adjacentNodes.begin(), current->adjacentNodes.end(), next) == current->adjacentNodes.end())
               current->adjacentNodes.push_back(next) ;
            
            if(find(next->adjacentNodes.begin(), next->adjacentNodes.end(), current) == next->adjacentNodes.end())
               next->adjacentNodes.push_back(current) ;
         }
         
         
         if (floorMapping[current->row][current->column + 1] == 0)
         {
            (current->downs).push_back(next) ;
            (next->ups).push_back(current) ;
            (next->backs).push_back(current) ;
            (next->backSteps) = current->backSteps + 1 ;
            (current->adjacentNodes).push_back(next) ;
            (next->adjacentNodes).push_back(current) ;

            // flag for done mapping
            floorMapping[current->row][current->column + 1] = 4 ;
            //push to queue for leter check for next node
            expendQueue.push(next) ;
         }
      }
      
      
      //check for the down node
      if (current->row + 1 < rows)
      {
         next = floorNodes[current->row + 1][current->column] ;

         
         if (floorMapping[current->row + 1][current->column] == 4)
         {
            if(find(current->ups.begin(), current->ups.end(), next) == current->ups.end())
               current->downs.push_back(next) ;
            if(find(next->downs.begin(), next->downs.end(), current) == next->downs.end())
               next->ups.push_back(current) ;
            
            // should consider 2 ways
            if (next->backSteps > current->backSteps + 1)
            {
               next->backSteps = current->backSteps + 1 ;
               next->backs.clear() ;
               next->backs.push_back(current) ;
            }
            
            if (next->backSteps == current->backSteps + 1)
               next->backs.push_back(current) ;
            
            if(find(current->adjacentNodes.begin(), current->adjacentNodes.end(), next) == current->adjacentNodes.end())
               current->adjacentNodes.push_back(next) ;
            
            if(find(next->adjacentNodes.begin(), next->adjacentNodes.end(), current) == next->adjacentNodes.end())
               next->adjacentNodes.push_back(current) ;
         }
         
         
         if (floorMapping[current->row + 1][current->column] == 0)
         {
            (current->downs).push_back(next) ;
            (next->ups).push_back(current) ;
            (next->backs).push_back(current) ;
            (next->backSteps) = current->backSteps + 1 ;
            (current->adjacentNodes).push_back(next) ;
            (next->adjacentNodes).push_back(current) ;

            // flag for done mapping
            floorMapping[current->row + 1][current->column] = 4 ;
            //push to queue for leter check for next node
            expendQueue.push(next) ;
         }
      }
      
      //done checking around this node, pop out of the queue
      expendQueue.pop() ;
      totalZeros++ ;
   }
   
   
   cout << endl ;
   cout << "Floor information:" << endl
        << "rows:     " << rows << endl
        << "columns:  " << columns << endl
        << "battery:  " << battery << endl << endl ;
   
   cout << "Running..." << endl << endl ;
   
   
   
   
   
   
   
   //find path
   
   Node *currentStep, *nextStep = NULL ;
   int currentBattery = battery ;
   bool backToLastEndPoint = 0 ;
   stack<Node*> backToLastEndPointPath ;
   vector<Node*> path ;
   
   
   currentStep = root ;
   
   while (totalZeros > 0)
   {
      if (totalSteps > 60000000)
         break ;
      
      //testing
      //cout << totalSteps << endl ;
      
      
      //explore map
      while (currentBattery > currentStep->backSteps)
      {
         //indicate came here
         currentStep->visitedTimes++ ;
         if (currentStep->status == 0)
         {
            currentStep->status = 2 ;
            totalZeros -- ;
         }
         path.push_back(currentStep) ;
         
         
         //find next
         nextStep = currentStep->adjacentNodes[0] ;
         
         for (int i = 0; i < currentStep->downs.size() ; i++)
         {
            if (currentStep->downs[i]->visitedTimes < nextStep->visitedTimes)
            {
               nextStep = currentStep->downs[i] ;
            }
         }
         
         for (int i = 0; i < currentStep->ups.size() ; i++)
         {
            if (currentStep->ups[i]->visitedTimes < nextStep->visitedTimes)
            {
               nextStep = currentStep->ups[i] ;
            }
         }
         
         for (int i = 0; i < currentStep->adjacentNodes.size() ; i++)
         {
            if (currentStep->adjacentNodes[i]->status == 0)
            {
               nextStep = currentStep->adjacentNodes[i] ;
               break ;
            }
         }
         
         //goto next
         currentStep = nextStep ;
         totalSteps ++ ;
         currentBattery -- ;
         
         if (totalZeros == 0) {
            break ;
         }
      }
      
      
      //backToLastEndPoint?
      for (int i = 0 ; i < currentStep->adjacentNodes.size() ; i++)
      {
         if (currentStep->adjacentNodes[i]->status == 0) {
            backToLastEndPoint = 1 ;
         }
      }
      
      
      
      
      
      
      //return to root
      while (currentStep != root)
      {
         //indicate came here
         currentStep->visitedTimes++ ;
         if (currentStep->status == 0)
         {
            currentStep->status = 2 ;
            totalZeros -- ;
         }
         path.push_back(currentStep) ;
         
         if (backToLastEndPoint) {
            backToLastEndPointPath.push(currentStep) ;
         }
         
         //find next
         nextStep = currentStep->backs[0] ;
         
         
         //goto next
         currentStep = nextStep ;
         totalSteps ++ ;
         currentBattery -- ;

      }
      
      
      // when back to root
      currentBattery = battery ;
      
      
      
      //back to last end point (if needed)
      if (backToLastEndPoint == 1)
      {
         backToLastEndPointPath.push(currentStep) ;
         
         while (backToLastEndPointPath.size() > 1 )
         {
            
            currentStep = backToLastEndPointPath.top() ;
            currentStep->visitedTimes++ ;
            if (currentStep->status == 0)
            {
               currentStep->status = 2 ;
               totalZeros -- ;
            }
            path.push_back(currentStep) ;

            //
            backToLastEndPointPath.pop() ;
            

            totalSteps ++ ;
            currentBattery -- ;
         
            
         }
         currentStep = backToLastEndPointPath.top() ;
         backToLastEndPointPath.pop() ;
         
         
         backToLastEndPoint = 0 ;
      }
   }
   
   
   
   if (path.back() != root)
   {
      //indicate came here
      currentStep->visitedTimes++ ;
      if (currentStep->status == 0)
      {
         currentStep->status = 2 ;
         totalZeros -- ;
      }
      path.push_back(currentStep) ;
      

      
      //find next
      nextStep = NULL ;
      
      
      //goto next
      currentStep = nextStep ;
      totalSteps ++ ;


   }
   

   
   
   
   /*
   
   //testing output
   
   for (int i = 0; i < rows; i++)
   {
      for (int j = 0; j < columns; j++)
      {
         current = floorNodes[i][j] ;
         
         if (current)
         {
            cout << current->visitedTimes << " " ;
         }
         else
         {
            cout << "   " ;
         }
         
      }
      cout << '\n' ;
   }
   
   

   for (int i = 0; i < rows; i++)
   {
      for (int j = 0; j < columns; j++)
      {
         current = floorNodes[i][j] ;
         
         if (current)
         {
            if(current->backs.size() > 0)
            {
               cout << current->backs[0] << " "  ;
               //cout << current->backSteps << "  "  ;
            }
            else
            {
               cout << "               " ;
            }
         }
         else
         {
            cout << "               " ;
         }
         
      }
      cout << '\n' ;
   }
   
   
   
   for (int i = 0; i < rows; i++)
   {
      for (int j = 0; j < columns; j++)
      {
         current = floorNodes[i][j] ;
         
         if (current->backs.size() > 0)
         {
            cout << current->backs[0] << " " ;
         }
         else
         {
            cout << "               " ;
         }
         
      }
      cout << '\n' ;
   }
   
   
   
   
   for (int i = 0 ; i < floorNodes[10][7]->ups.size() ; i++)
   {
      cout << floorNodes[10][7]->ups[i] << endl ;
   }
   */
   

   
   cout << "0's left: " << totalZeros << endl
        << "Steps:    " << totalSteps << endl << endl ;
   
   
   
   
   
   
   
   ofstream output ;
   output.open("final.path", ios::trunc) ;
   
   if (output.is_open())
   {
      
      output << totalSteps << endl ;
      
      
      for (int i = 0; i < path.size() ; i++)
      {
         output << path[i]->row << " " << path[i]->column << endl ;
      }
      
      output.close() ;
      
      cout << "The result is output to the file name: " << "final.path" << endl ;
   }
   else
   {
      cout << "Unable to output file." << endl ;
   }
   
   
   
   
   
   
   
   
   
   
   
   return 0;
}





