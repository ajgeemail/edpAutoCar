import java.util.*;

/**
 * Navigation software subsystem for an automated vehicle system that takes the position of
 * the system and a grid of detected obstacles to move from a start position to an end
 * position
 *
 * @author Brady Walker
 * @version 1.0
 */
public class navigate
{
    private static int height;
    private static int length;
    private static int xPosEnd;
    private static int yPosEnd;
    // start position is only needed in move methods
    private static int xPosStart; 
    private static int yPosStart;
    private static int xPosCurrent;
    private static int yPosCurrent;
    private int yPosStartArr;
    private int yPosEndArr;
    private int yPosCurrentArr;
    gridSquare[][] grid;
    gridSquare gridSquareNorth;
    gridSquare gridSquareEast;
    gridSquare gridSquareSouth;
    gridSquare gridSquareWest;
    Stack <Integer> moveHistory = new Stack <Integer>();
    
    /**
     * Constructor for objects of class navigate
     */    
    public navigate(){
        xPosEnd = 9;
        yPosEnd = 9;
        xPosStart = 1;
        yPosStart = 1;
        length = 10;
        height = 10;
    }
    
    /**
     * s
     * param  args  Arguements
     */  
    public static void main(String[] args){
        long startTime = System.nanoTime();
        navigate nav = new navigate();
        
        nav.createGrid(height, length);
        nav.test();
        nav.prioritiseGrid(xPosEnd, yPosEnd);
        nav.printGrid();
        xPosCurrent = 0;
        yPosCurrent = 0;
        while(xPosCurrent != xPosEnd && yPosCurrent != height - yPosEnd + 1){
            nav.move();
        }
        nav.printMoveHistory();
        
        // ends the timer
        long endTime   = System.nanoTime();
        long totalTime = (endTime - startTime) / 1000;
        System.out.println("\nElapsed time: " + totalTime + "us");
    }
    
    /**
     * Creates a grid that with a given length, height and a boundary wall of obstacles
     * 
     * @param  height  Height of the grid to be traversed
     * @param  length  Length of the grid to be traversed
     */
    public void createGrid(int height, int length){
        // the additional two units to each dimensions is to create an obstacle boundary 
        // wall around the grid
        grid = new gridSquare[height + 2][length + 2];
        
        for(int i = 0; i < grid.length; i++){
            for(int j = 0; j < grid[i].length; j++){
                grid[i][j] = new gridSquare();
                grid[i][j].xPos = j;
                grid[i][j].yPos = i; 
                grid[i][j].yPos = height - i + 1; // converts to cartesian coords
            }
        }
        
        // assigns wall elements with obstacle priority values
        for(int i = 0; i < grid.length; i++){
            grid[0][i].value = 4000;
            grid[i][0].value = 4000;
            grid[height + 1][i].value = 4000;
            grid[i][length + 1].value = 4000;
        }
    }
    
    /**
     * Prioritises each grid square using the flood-fill algorithm
     * 
     * @param  xPosEnd  Horizontal position of the End grid square
     * @param  yPosEnd  Vertical position of the End grid square
     */
    public void prioritiseGrid(int xPosEnd, int yPosEnd){
        Queue <gridSquare> gridQueue = new LinkedList <gridSquare>();
        // Grid square currently being prioritised
        gridSquare gridSquareCurrent = new gridSquare();
       
        yPosEndArr = height - yPosEnd + 1;       // Converts from gid to array coordinates
        grid[yPosEndArr][xPosEnd].value = 0;
        gridQueue.add(grid[yPosEndArr][xPosEnd]);
        
        while(!gridQueue.isEmpty()){
            gridSquareCurrent = gridQueue.poll();   
            xPosCurrent = gridSquareCurrent.xPos;    
            // Converts from grid to array coordinates
            yPosCurrentArr = height + 1 - gridSquareCurrent.yPos; 
            
            // Grid squares with cardinal directions from the current grid square
            gridSquareNorth = grid[yPosCurrentArr + 1][xPosCurrent];
            gridSquareEast = grid[yPosCurrentArr][xPosCurrent + 1];
            gridSquareSouth = grid[yPosCurrentArr - 1][xPosCurrent];
            gridSquareWest = grid[yPosCurrentArr][xPosCurrent - 1];
            
            // Assigns a value to an unassigned north cell and adds it to the queue
            if(gridSquareNorth.value == -1){
                gridSquareNorth.value = gridSquareCurrent.value + 1;        
                gridQueue.add(gridSquareNorth);
            }
            // Assigns a value to an unassigned east cell and adds it to the queue
            if(gridSquareEast.value == -1){
                gridSquareEast.value = gridSquareCurrent.value + 1;        
                gridQueue.add(gridSquareEast);
            }
            // Assigns a value to an unassigned south cell and adds it to the queue
            if(gridSquareSouth.value == -1){
                gridSquareSouth.value = gridSquareCurrent.value + 1;        
                gridQueue.add(gridSquareSouth);
            }
            // Assigns a value to an unassigned west cell and adds it to the queue
            if(gridSquareWest.value == -1){
                gridSquareWest.value = gridSquareCurrent.value + 1;        
                gridQueue.add(gridSquareWest);
            }   
        }
    } 
    
    /**
     * Determines movement with respect to the current grid square and neighbouring values
     * 
     * @param  xPos  Current horizontal position 
     * @param  yPos  Current vertical position
     * need to change to be parseable with xpos and ypos ints
     */
    public int move(){
        if(moveHistory.empty()){
            xPosCurrent = xPosStart;
            yPosCurrentArr = height + 1 - yPosStart; // Converts to array co-ordinates
        }        
        System.out.println("\nCurrent x: " + xPosCurrent + "\t Current yArr:" + yPosCurrentArr + "\tCurrent value: " + grid[yPosCurrentArr][xPosCurrent].value);

        // Values of grid squares with cardinal directions from the current grid square
        int gridNorthVal = grid[yPosCurrentArr - 1][xPosCurrent].value;
        int gridSouthVal = grid[yPosCurrentArr + 1][xPosCurrent].value;
        int gridEastVal = grid[yPosCurrentArr][xPosCurrent + 1].value;
        int gridWestVal = grid[yPosCurrentArr][xPosCurrent - 1].value;
        
        if(yPosCurrentArr != height - yPosEnd + 1){
            if(gridNorthVal <= gridEastVal && gridNorthVal <= gridWestVal && 
               gridNorthVal <= gridSouthVal){           
                moveHistory.push(1);
                updatePosCurrent(xPosCurrent, yPosCurrentArr - 1); // -1 indicates north movement in array
                System.out.println("Forward");
                return 1; // Indicates a forward movement        
            }
            else if(gridSouthVal <= gridNorthVal && gridSouthVal <= gridEastVal && 
                    gridSouthVal <= gridWestVal){
                moveHistory.push(4);
                updatePosCurrent(xPosCurrent, yPosCurrentArr + 1); // +1 indicates south movement in array
                System.out.println("Reverse");
                return 4; // Indicates a rear movement 
            }
        }
        if(gridEastVal <= gridNorthVal && gridEastVal <= gridSouthVal && 
           gridEastVal <= gridWestVal){
            moveHistory.push(2);
            updatePosCurrent(xPosCurrent + 1, yPosCurrentArr);
            System.out.println("Right Turn");
            return 2; // Indicates a right movement        
        }
        else if(gridWestVal <= gridNorthVal && gridWestVal <= gridEastVal && 
            gridWestVal <= gridSouthVal){
            moveHistory.push(3);
            updatePosCurrent(xPosCurrent - 1, yPosCurrentArr);
            System.out.println("Left Turn");
            return 3; // Indicates a left movement        
        }
        return -1;
    }
    
    /**
     * Updates current position variables
     * 
     * @param  xPos  Current horizontal position 
     * @param  yPos  Current vertical position
     */
    public void updatePosCurrent(int xPos, int yPos){
        xPosCurrent = xPos;
        yPosCurrentArr = yPos;
    }
    
    /**
     * Prints the grid with the associated priority value for each grid square
     */
    public void printGrid(){
        for(int i = 0; i < grid.length; i++){
            for(int j = 0; j < grid[i].length; j++){
                System.out.print(grid[i][j].value + "\t");
            }
            System.out.println();
        }   
    }
    
    /**
     * Prints the movement history
     */
    public void printMoveHistory(){
        System.out.print("Moves: \t");
        while(!moveHistory.isEmpty()){
            System.out.print(moveHistory.pop() + ",  ");
        }   
    }
    
    public void test(){
        grid[height + 1 - 10][4].value = 4000;
        grid[height + 1 - 10][5].value = 4000;
        grid[height + 1 - 10][6].value = 4000;
        grid[height + 1 - 10][7].value = 4000;
        grid[height + 1 - 8][1].value = 4000;
        grid[height + 1 - 8][2].value = 4000;
        grid[height + 1 - 8][3].value = 4000;
        grid[height + 1 - 8][4].value = 4000;
        grid[height + 1 - 8][5].value = 4000;
        grid[height + 1 - 8][6].value = 4000;
        grid[height + 1 - 4][3].value = 4000;
        grid[height + 1 - 5][3].value = 4000;
        grid[height + 1 - 3][8].value = 4000;
        grid[height + 1 - 3][9].value = 4000;
        grid[height + 1 - 3][10].value = 4000;
        grid[height + 1 - 6][6].value = 4000;
        grid[height + 1 - 6][7].value = 4000;
        grid[height + 1 - 6][8].value = 4000;
        grid[height + 1 - 5][7].value = 4000;
    }
}