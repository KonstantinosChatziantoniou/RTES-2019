#ifndef AWARENODES
#define AWARENODES



typedef struct aware_nodes {
    char** nodes;
    int length;
}AwareNodes;




/**
 * Returns 1, if the node already exists in the AwareNodes List
 * 
 */
int CheckDuplicate(AwareNodes an, char* nd);



/**
 * Adds a new node to the Aware Nodes List
 * 
 */
void AddNewAwareNode(AwareNodes *an, char* node);



/**
 * Checks if the node already exists in the Aware Nodes List, and 
 * if it doesn't, it is added.
 * Returns 1 if it was added, 0 if it wasn't (duplicate)
 * 
 */
int CheckAndAddNode(AwareNodes *an, char* node);




void PrintAwareNodes(AwareNodes an);

void InitAwareNodes(AwareNodes *an);

void DeleteAwareNodes(AwareNodes *an);


void PrintKnownNodes();
void setUpKnownNodes();
AwareNodes GetKnwonNodes();
AwareNodes* ReturnUnawareNodes(AwareNodes nd);

#endif