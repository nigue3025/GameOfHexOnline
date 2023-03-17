#include<string>
#include<vector>
#include<stack>
#include<functional>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include<time.h>

class Node
{

public:
	enum EnumNodePlayerOrderStatus :short { Empty, FirstPlayer, SecondPlayer };
	EnumNodePlayerOrderStatus NodeStatus;

	Node(int i, int j, int size, std::vector<int>& edgelist)
	{
		cord_i = i;
		cord_j = j;
		this->size = size;
		MyEdgeList = edgelist;
		No = i * size + j;
		NodeStatus = EnumNodePlayerOrderStatus::Empty;
	};
	int cord_i, cord_j, size;
	int No;

	std::vector<int> MyEdgeList;

	std::string showCordinate()
	{

		std::string str;
		str.append("(" + std::to_string(cord_i) + "," + std::to_string(cord_j) + ")");
		return str;
	}

	Node clone()
	{
		std::vector<int> tempEdgeList = MyEdgeList;
		Node tempNode(cord_i, cord_j, size, tempEdgeList);
		return tempNode;
	}
	~Node()
	{
		auto k = this;
		k = k;

	}



};



class HexGraph
{
public:
	int count = 0;
	int size;
	std::vector<Node*> NodeList;
	//std::vector<Node*> FirstPlayerNodeList;
	//std::vector<Node*> SecondPlayerNodeList;

	std::map<int, Node*> RemainedEmptyNode;

	std::unordered_set<std::string> CordinateHashSet;
	std::unordered_map<std::string, Node*> NodeMap;

	enum EnumEdgeCornerIternal :short { Internal, LeftEdge, RightEdge, TopEdge, BotEdge, TopLeftCorner, TopRightCorner, BotLeftCorner, BotRightCorner, Error };
	enum EnumNodeStatus :short { Empty, FirstPlayer, SecondPlayer };
	enum EnumPlayResult :short { StillPlaying, FirstPlayerWinToken, SecondPlayerWinToken, DrawGame };
	Node* LastNodePlaced = nullptr;

	std::vector<std::string> RedBlueEdgeNodeStrLst;


	void Restore(HexGraph& hexGraph)
	{

		this->LastNodePlaced = hexGraph.LastNodePlaced;
		this->placeCount = hexGraph.placeCount;
		this->HasTopRedEdgePlaced = hexGraph.HasTopRedEdgePlaced;
		this->HasBotRedEdgePlaced = hexGraph.HasBotRedEdgePlaced;
		this->HasLeftBlueEdgePlaced = hexGraph.HasLeftBlueEdgePlaced;
		this->HasRightBlueEdgePlaced = hexGraph.HasRightBlueEdgePlaced;

		for (auto& node : this->NodeMap)
			this->NodeMap[node.first]->NodeStatus = hexGraph.NodeMap[node.first]->NodeStatus;

		this->RemainedEmptyNode.clear();
		for (auto& node : hexGraph.RemainedEmptyNode)
			this->RemainedEmptyNode[node.first] = hexGraph.NodeMap[node.second->showCordinate()];

	}

	HexGraph clone()
	{
		HexGraph tempHexGraph = HexGraph(size);


		tempHexGraph.LastNodePlaced = this->LastNodePlaced;
		tempHexGraph.placeCount = this->placeCount;

		tempHexGraph.HasTopRedEdgePlaced = this->HasTopRedEdgePlaced;
		tempHexGraph.HasBotRedEdgePlaced = this->HasBotRedEdgePlaced;
		tempHexGraph.HasLeftBlueEdgePlaced = this->HasLeftBlueEdgePlaced;
		tempHexGraph.HasRightBlueEdgePlaced = this->HasRightBlueEdgePlaced;

		std::unordered_map<std::string, Node*> tempNodeMap;

		for (auto& node : this->NodeMap)
			tempHexGraph.NodeMap[node.first]->NodeStatus = node.second->NodeStatus;

		tempHexGraph.RemainedEmptyNode.clear();
		for (auto& node : this->RemainedEmptyNode)
			tempHexGraph.RemainedEmptyNode[node.first] = tempHexGraph.NodeMap[node.second->showCordinate()];




		return tempHexGraph;


	}

	~HexGraph()
	{
		NodeList = NodeList;
		NodeMap = NodeMap;

	}

	HexGraph() {};
	HexGraph(int size)
	{
		EnumEdgeCornerIternal ec = EnumEdgeCornerIternal::Internal;
		this->size = size;
		edgelist.resize(size * size);
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				Node* NewNode = makeNode(i, j, size, edgelist[count++]);
				CordinateHashSet.insert(NewNode->showCordinate());
				NodeList.push_back(NewNode);
				NodeMap[NewNode->showCordinate()] = NewNode;
				RemainedEmptyNode[NewNode->No] = NewNode;
			}

		generateRedBlueEdgeNodeStrLst();

	}

	void generateRedBlueEdgeNodeStrLst()
	{
		for (int i = 0; i < size; i++)
		{
			auto TopEdge = "(0," + std::to_string(i) + ")";
			auto LeftEdge = "(" + std::to_string(i) + ",0)";
			auto BotEdge = "(" + std::to_string(size - 1) + "," + std::to_string(i) + ")";
			auto rightEdge = "(" + std::to_string(i) + "," + std::to_string(size - 1) + ")";

			RedBlueEdgeNodeStrLst.push_back(TopEdge);
			RedBlueEdgeNodeStrLst.push_back(LeftEdge);
			RedBlueEdgeNodeStrLst.push_back(BotEdge);
			RedBlueEdgeNodeStrLst.push_back(rightEdge);
		}
	}
	//Dislay the player of the game with the last move
	Node::EnumNodePlayerOrderStatus LastMovedPlayer()
	{
		if ((placeCount + 1) % 2 == 1)
			return Node::EnumNodePlayerOrderStatus::SecondPlayer;
		else
			return Node::EnumNodePlayerOrderStatus::FirstPlayer;

	}

	//Dislay the player of the game with the next move
	Node::EnumNodePlayerOrderStatus NextMovePlayer()
	{
		if ((placeCount + 1) % 2 == 1)
			return Node::EnumNodePlayerOrderStatus::FirstPlayer;
		else
			return Node::EnumNodePlayerOrderStatus::SecondPlayer;
	}



	bool HasTopRedEdgePlaced = false;
	bool HasBotRedEdgePlaced = false;
	bool HasTopBotRedEdgePlaced()
	{
		return HasTopRedEdgePlaced && HasBotRedEdgePlaced;
	}
	void UpdateTopDownRedEdge(Node& node)
	{
		auto position = JudgeEdgeCornerInteriorPoistion(node.cord_i, node.cord_j, size);
		if (HasTopRedEdge(position))
		{
			HasTopRedEdgePlaced = true;

		}
		if (HasBotRedEdge(position))
		{
			HasBotRedEdgePlaced = true;

		}

	}

	bool HasLeftRightBlueEdgePlaced = false;
	bool HasLeftBlueEdgePlaced = false;
	bool HasRightBlueEdgePlaced = false;

	bool HasLeftRightBLueEdgePlaced()
	{
		return HasLeftBlueEdgePlaced && HasRightBlueEdgePlaced;
	}

	void UpdateLeftRightBlueEdge(Node& node)
	{
		auto position = JudgeEdgeCornerInteriorPoistion(node.cord_i, node.cord_j, size);
		if (HasLeftBlueEdge(position))
		{
			HasLeftBlueEdgePlaced = true;

		}
		if (HasRIghtBlueEdge(position))
		{
			HasRightBlueEdgePlaced = true;

		}

	}


	//For player move, input format with "(i,j)" is strictly limited
	bool place(std::string cordStr, Node::EnumNodePlayerOrderStatus NodeStatus, std::string& msg)
	{
		msg = "";
		if (NodeStatus == EnumNodeStatus::Empty)
		{
			msg = "Empty as player role is not allowed";
			return false;
		}
		else if (NodeMap.find(cordStr) == NodeMap.end())
		{
			msg = "Cordinate" + cordStr + " does not exists";
			return false;
		}
		else if (NodeMap[cordStr]->NodeStatus != Node::EnumNodePlayerOrderStatus::Empty)
		{
			msg = "Cordinate " + cordStr + " is occupied already, try again.";
			return false;
		}

		NodeMap[cordStr]->NodeStatus = NodeStatus;


		LastNodePlaced = NodeMap[cordStr];

		++placeCount;


		UpdateLeftRightBlueEdge(*LastNodePlaced);
		UpdateTopDownRedEdge(*LastNodePlaced);
		RemainedEmptyNode.erase(LastNodePlaced->No);

		RemainedEmptyNode = RemainedEmptyNode;
		return true;
	}





	//Display board with player move
	std::string showArrangedPlayerNodes()
	{
		return showArrangedBoard([](Node* a) {
			std::string player = ".";
			if (a->NodeStatus == EnumNodeStatus::FirstPlayer)
				player = "X";
			else if (a->NodeStatus == EnumNodeStatus::SecondPlayer)
				player = "O";
			if (a->cord_j < a->size - 1)
				return " " + player + " -";
			return " " + player + "  ";
			});
	}

	//Display board with edge size of each node
	std::string showArrangedNodesEdgeSize()
	{
		return showArrangedBoard([](Node* a) {
			if (a->cord_j < a->size - 1)	return " " + std::to_string(a->MyEdgeList.size()) + " -";
			return " " + std::to_string(a->MyEdgeList.size()) + "  ";
			});
	}

	//Display board with each node id
	std::string showArrangedNodesId()
	{
		return showArrangedBoard([](Node* a) {
			if (a->cord_j < a->size - 1)	return " " + std::to_string(a->No) + " -";
			return " " + std::to_string(a->No) + "  ";
			});

	}


	//Display board with each node cordinate
	std::string showArrangedNodesCordinates()
	{
		return showArrangedBoard([](Node* a) {
			if (a->cord_j < a->size - 1)
				return a->showCordinate() + "-";
			return a->showCordinate();

			});
	}

	HexGraph::EnumPlayResult JudgeWinner()
	{
		auto stmHxGrph = *this;
		HexGraph::EnumPlayResult rslt;
		std::string msg2;
		for (auto& node : stmHxGrph.RedBlueEdgeNodeStrLst)
		{
			auto tempNode = stmHxGrph.NodeMap[node];
			if (stmHxGrph.JudgeWinner(*tempNode, msg2, rslt, tempNode->NodeStatus))
			{

				if (rslt == HexGraph::DrawGame)
					continue;
				else
				{

				}

				return rslt;

			}

		}
		return HexGraph::DrawGame;
	}



	bool JudgeWinner(Node& JustPlacedNode, std::string& msgOut, HexGraph::EnumPlayResult& result, Node::EnumNodePlayerOrderStatus player)
	{
		player = JustPlacedNode.NodeStatus;
		if (JudgeWinner(JustPlacedNode, msgOut, player))
		{
			if (player == Node::EnumNodePlayerOrderStatus::FirstPlayer)
				result = HexGraph::EnumPlayResult::FirstPlayerWinToken;
			else //if(LastMovedPlayer() == Node::EnumNodePlayerStatus::SecondPlayer)
				result = HexGraph::EnumPlayResult::SecondPlayerWinToken;


			return true;
		}

		if (placeCount == size * size)
		{
			result = HexGraph::EnumPlayResult::DrawGame;
			return true;
		}
		result = HexGraph::EnumPlayResult::StillPlaying;
		return false;

	}


	bool HasLeftBlueEdge(HexGraph::EnumEdgeCornerIternal a)
	{

		if (a == HexGraph::EnumEdgeCornerIternal::LeftEdge ||
			a == HexGraph::EnumEdgeCornerIternal::TopLeftCorner ||
			a == HexGraph::EnumEdgeCornerIternal::BotLeftCorner)
			return true;
		return false;
	}

	bool HasRIghtBlueEdge(HexGraph::EnumEdgeCornerIternal a)
	{
		if (a == HexGraph::EnumEdgeCornerIternal::RightEdge ||
			a == HexGraph::EnumEdgeCornerIternal::TopRightCorner ||
			a == HexGraph::EnumEdgeCornerIternal::BotRightCorner)
			return true;
		return false;

	}

	bool HasTopRedEdge(HexGraph::EnumEdgeCornerIternal a)
	{
		if (a == HexGraph::EnumEdgeCornerIternal::TopEdge ||
			a == HexGraph::EnumEdgeCornerIternal::TopLeftCorner ||
			a == HexGraph::EnumEdgeCornerIternal::TopRightCorner)
			return true;
		return false;
	}

	bool HasBotRedEdge(HexGraph::EnumEdgeCornerIternal a)
	{
		if (a == HexGraph::EnumEdgeCornerIternal::BotEdge ||
			a == HexGraph::EnumEdgeCornerIternal::BotLeftCorner ||
			a == HexGraph::EnumEdgeCornerIternal::BotRightCorner)
			return true;
		return false;

	}

	bool JudgeWinner(Node& JustPlacedNode, std::string& msgOut, Node::EnumNodePlayerOrderStatus& player)
	{
		msgOut = "player " + showPlayer(player) + " ";

		std::set<int> nodeVisistedList;
		std::stack<int> stackedEdge;
		std::stack<std::vector<int>> stackedCandidates;

		if (placeCount < 2 * size - 1)
			return false;

		if (player == Node::EnumNodePlayerOrderStatus::FirstPlayer)
		{
			if (!HasLeftRightBLueEdgePlaced())
				return false;
		}
		else
		{
			if (!HasTopBotRedEdgePlaced())
				return false;
		}
		auto NodeToVisit = JustPlacedNode;
		nodeVisistedList.insert(JustPlacedNode.No);



		while (true) //Enlist all the nodes which can be connected to the "JustPlacedNode" directly or indirectly 
		{
			for (auto anEdge : NodeToVisit.MyEdgeList)
			{
				if (NodeList[anEdge]->NodeStatus == JustPlacedNode.NodeStatus)  //Make sure the adjacent node has occoupied by the same player
				{
					if (nodeVisistedList.find(anEdge) == nodeVisistedList.end())  //Do not revisit the nodes
					{
						nodeVisistedList.insert(anEdge);
						stackedEdge.push(anEdge);
					}
				}
			}
			if (stackedEdge.size() > 0)
			{
				NodeToVisit = *NodeList[stackedEdge.top()];
				stackedEdge.pop();
			}
			else
				break;
		}


		bool HasEdge1 = false;
		bool HasEdge2 = false;
		for (auto& aNode : nodeVisistedList) // To make sure if the "nodeVisitedList" contains two opposite site edges for winning criteria assigned for different player 
		{
			int cordinate_i = aNode / size;
			int cordinate_j = aNode % size;
			auto position = JudgeEdgeCornerInteriorPoistion(cordinate_i, cordinate_j, size);

			if (player == Node::EnumNodePlayerOrderStatus::FirstPlayer)
			{
				if (HasLeftBlueEdge(position))
					HasEdge1 = true;
				else if (HasRIghtBlueEdge(position))
					HasEdge2 = true;
			}
			else
			{
				if (HasTopRedEdge(position))
					HasEdge1 = true;
				else if (HasBotRedEdge(position))
					HasEdge2 = true;
			}

		}



		return HasEdge1 && HasEdge2;
	}

	bool JudgeWinner(Node& JustPlacedNode, std::string& msgOut)
	{

		auto thePlayer = this->LastMovedPlayer();
		return JudgeWinner(JustPlacedNode, msgOut, thePlayer);

	}

	int getPlaceCount()
	{
		return placeCount;
	}

private:
	std::vector<std::vector<int>> edgelist;

	int placeCount = 0;
	//Judge the node position(Edge,Corner or Internal)
	HexGraph::EnumEdgeCornerIternal JudgeEdgeCornerInteriorPoistion(Node node) const
	{
		return JudgeEdgeCornerInteriorPoistion(node.cord_i, node.cord_j, node.size);
	}

	//Judge the node position(Edge,Corner or Internal)
	HexGraph::EnumEdgeCornerIternal JudgeEdgeCornerInteriorPoistion(int i, int j, int size) const
	{

		//if (i == j && i != 0 && i != size - 1)

		if (i == size - 1 && j == size - 1)
			return EnumEdgeCornerIternal::BotRightCorner;
		else if (i == 0 && j == 0)
			return EnumEdgeCornerIternal::TopLeftCorner;

		else if (i == 0 && j == size - 1)
			return EnumEdgeCornerIternal::TopRightCorner;
		else if (i == size - 1 && j == 0)
			return EnumEdgeCornerIternal::BotLeftCorner;
		else if (i == 0 && j > i)
			return EnumEdgeCornerIternal::TopEdge;
		else if (j == 0)
			return EnumEdgeCornerIternal::LeftEdge;
		else if (j == size - 1)
			return EnumEdgeCornerIternal::RightEdge;
		else if (i == size - 1)
			return EnumEdgeCornerIternal::BotEdge;
		return EnumEdgeCornerIternal::Internal;
	}


	Node* makeNode(int i, int j, int size, std::vector<int>& theEdgelist)
	{
		Node* node = new Node(i, j, size, theEdgelist);

		auto position = JudgeEdgeCornerInteriorPoistion(*node);

		//Assign the node edge base on the poisition judged
		switch (position)
		{
		case EnumEdgeCornerIternal::TopLeftCorner:
			node->MyEdgeList.push_back(node->cord_i + 1);
			node->MyEdgeList.push_back(node->size);
			break;
		case EnumEdgeCornerIternal::TopRightCorner:
			node->MyEdgeList.push_back(node->size - 2);
			node->MyEdgeList.push_back(node->size + node->size - 1);
			node->MyEdgeList.push_back(node->size + node->size - 2);
			break;
		case EnumEdgeCornerIternal::BotLeftCorner:
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1));
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + 1);
			break;
		case EnumEdgeCornerIternal::BotRightCorner:
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->size - 2);
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->size - 1);
			break;
		case EnumEdgeCornerIternal::TopEdge:
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j + 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j - 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i + 1) + node->cord_j - 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i + 1) + node->cord_j);
			break;
		case EnumEdgeCornerIternal::LeftEdge:
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->cord_j);
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->cord_j + 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i + 1) + node->cord_j);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j + 1);
			break;
		case EnumEdgeCornerIternal::RightEdge:
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->cord_j);
			node->MyEdgeList.push_back(node->size * (node->cord_i + 1) + node->cord_j);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j - 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i + 1) + node->cord_j - 1);
			break;
		case EnumEdgeCornerIternal::BotEdge:
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->cord_j);
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->cord_j + 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j + 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j - 1);
			break;
		case EnumEdgeCornerIternal::Internal:
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->cord_j);
			node->MyEdgeList.push_back(node->size * (node->cord_i + 1) + node->cord_j);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j - 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i + 1) + node->cord_j - 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i - 1) + node->cord_j + 1);
			node->MyEdgeList.push_back(node->size * (node->cord_i) + node->cord_j + 1);

			break;
		}
		return node;
	}
	std::string showPlayer(Node::EnumNodePlayerOrderStatus status)
	{
		if (status == Node::EnumNodePlayerOrderStatus::FirstPlayer)
			return "Player 1(Blue) ";
		else if (status == Node::EnumNodePlayerOrderStatus::SecondPlayer)
			return "Player2(Red) ";
		else
			return "error";
	}

	std::string showArrangedBoard(std::string formatInput(Node* node))
	{
		std::string str;
		int prevCordi = 0;
		int ident_count = 0;
		std::vector<int> elementStrSz;

		std::string IdentStr = " ";
		for (auto& node : NodeList)
		{
			if (node->cord_i - prevCordi > 0)
			{
				ident_count++;
				str.append("\n");
				for (int i = 0; i < ident_count; i++)
					str.append(IdentStr);

				prevCordi = node->cord_i;

			}
			auto outputStr = formatInput(node);
			str.append(outputStr);
			elementStrSz.push_back(outputStr.size() - 1);


			if (node->cord_j == node->size - 1)
			{
				if (node->cord_i == node->size - 1)break;

				str.append("\n");
				for (int i = 0; i < ident_count; i++)
					str.append(IdentStr);


				for (int i = 0; i < node->size; i++)
				{
					for (int j = 0; j < elementStrSz[i] - 2; j++)
						str.append(" ");
					str.append("\\");
					if (i < node->size - 1)
						str.append(" /");
				}
				elementStrSz.clear();
			}

		}


		return str;
	}



};
class AI
{
public:
	Node::EnumNodePlayerOrderStatus Player;
	AI(Node::EnumNodePlayerOrderStatus player)
	{
		Player = player;
		srand(time(NULL));
	}

	virtual std::string generateNextMove(HexGraph& hexGrpah)
	{
		return "";
	}
protected:

	int drawCandidateMove(HexGraph& hexGraph)
	{
		int moveCandidates = hexGraph.RemainedEmptyNode.size();
		if (moveCandidates == 0)
			return -1; //No More next move

		auto toPickNo = hexGraph.RemainedEmptyNode.begin();
		std::advance(toPickNo, rand() % moveCandidates);
		return toPickNo->second->No;

	}

	Node* drawCandidateMoveNode(HexGraph& hexGraph)
	{
		auto toPickNo = hexGraph.RemainedEmptyNode.begin();
		std::advance(toPickNo, rand() % hexGraph.RemainedEmptyNode.size());
		return toPickNo->second;
	}


};
class RandomMethod :public AI
{
public:
	RandomMethod(Node::EnumNodePlayerOrderStatus player) :AI(player)
	{
	};

	std::string generateNextMove(HexGraph& hexGraph) override
	{
		int NextMoveNo = drawCandidateMove(hexGraph);
		if (NextMoveNo == -1)//No more next move
			return "";
		auto selectedNode = hexGraph.RemainedEmptyNode[NextMoveNo];
		return selectedNode->showCordinate();
	}
};

class MonteCarloMethod :public AI
{
public:

	MonteCarloMethod(int simulateNo, Node::EnumNodePlayerOrderStatus player) : AI(player)
	{
		SimulateNo = simulateNo;
		//randomMovement =new RandomMethod(player);
	};




	std::string generateNextMove(HexGraph& hexGraph) override
	{
		auto bestMove = getBestMove(hexGraph);
		return hexGraph.RemainedEmptyNode[bestMove]->showCordinate();
	}




private:
	int SimulateNo;

	void SimulationThread(HexGraph& givenHexGraph, const int& index, const std::string& cordinate, int* player1WinCountBoard, int* player2WinCountBoard)
	{
		int bestWinCount = -1;
		int WinCount_secondPlan = -1;
		Node* bestMove = nullptr;
		Node* BestMove_secondPlan = nullptr;

		std::string msg;
		HexGraph tmpHexGraph = givenHexGraph.clone();
		auto candidate = givenHexGraph.RemainedEmptyNode[index];

		tmpHexGraph.place(cordinate, tmpHexGraph.NextMovePlayer(), msg);
		HexGraph::EnumPlayResult theSimWinner, theSimLoser;
		int curWinnerWinCount = -1;
		int curLoserWinCount = -1;
		runBatchSimulation(tmpHexGraph, theSimWinner, curWinnerWinCount, theSimLoser, curLoserWinCount);


		if (theSimWinner == HexGraph::EnumPlayResult::FirstPlayerWinToken)
		{
			player1WinCountBoard[index] = curWinnerWinCount;
			player2WinCountBoard[index] = curLoserWinCount;
		}
		else
		{
			player1WinCountBoard[index] = curLoserWinCount;
			player2WinCountBoard[index] = curWinnerWinCount;

		}
	}

	void DisplayProgess(int currCount, int totalCount)
	{
		int Q1 = (double)totalCount * 0.25;
		int Q2 = (double)totalCount * 0.5;
		int Q3 = (double)totalCount * 0.75;
		int Q4 = totalCount - 1;

		if (currCount == Q1)
			std::cout << "MonteCarlo Progress:25% complete...\n";
		else if (currCount == Q2)
			std::cout << "MonteCarlo Progress:50% complete...\n";
		else if (currCount == Q3)
			std::cout << "MonteCarlo Progress:75% complete...\n";
		else if (currCount == Q4)
			std::cout << "MonteCarlo Progress:100% complete...\n";


	}

	int getBestMove(HexGraph& givenHexGraph)
	{
		int bestWinCount = -1;
		int WinCount_secondPlan = -1;
		Node* bestMove = nullptr;
		Node* BestMove_secondPlan = nullptr;
		int loopCount = 0;
		int totalStepToGo = givenHexGraph.RemainedEmptyNode.size();

		if (givenHexGraph.RemainedEmptyNode.size() == 1) //Remained last move only. No longer to be simulated with other moves
			return givenHexGraph.RemainedEmptyNode.begin()->second->No;
		for (auto& candidate : givenHexGraph.RemainedEmptyNode) //Run simulation for evaluating each empty move to get the best move
		{
			std::string msg;
			HexGraph tmpHexGraph = givenHexGraph.clone();
			tmpHexGraph.place(candidate.second->showCordinate(), tmpHexGraph.NextMovePlayer(), msg);
			HexGraph::EnumPlayResult theSimWinner, theSimLoser;
			int curWinnerWinCount = -1;
			int curLoserWinCount = -1;
			runBatchSimulation(tmpHexGraph, theSimWinner, curWinnerWinCount, theSimLoser, curLoserWinCount);
			if (theSimWinner == givenHexGraph.NextMovePlayer())
			{
				if (curWinnerWinCount > bestWinCount)
				{
					bestWinCount = curWinnerWinCount;
					bestMove = candidate.second;
				}
			}
			else
			{
				if (curLoserWinCount > WinCount_secondPlan) //For second plan(Avoid generating poor move if opponent win count dominates)
				{
					WinCount_secondPlan = curLoserWinCount;
					BestMove_secondPlan = candidate.second;
				}
			}

			DisplayProgess(++loopCount, totalStepToGo);
		}
		if (bestMove == nullptr)
		{
			if (BestMove_secondPlan != nullptr)
				bestMove = BestMove_secondPlan; //For second plan(Avoid generating poor move if opponent win count dominates)
			else
				bestMove = givenHexGraph.RemainedEmptyNode.begin()->second; //Extreme case... No more possible moves for winning
		}
		return bestMove->No;
	}



	void runBatchSimulation(HexGraph& hexGraph, HexGraph::EnumPlayResult& winner, int& winnerWinCount, HexGraph::EnumPlayResult& loser, int& loserWinCount)
	{
		std::map<HexGraph::EnumPlayResult, int> PlayerWinCount;
		PlayerWinCount[HexGraph::EnumPlayResult::FirstPlayerWinToken] = 0;
		PlayerWinCount[HexGraph::EnumPlayResult::SecondPlayerWinToken] = 0;

		auto HexGraphForRestore = hexGraph.clone(); //Just clone HexGraph one time for restoring HexGraph data after each simulatin run done
		if (hexGraph.RemainedEmptyNode.size() == 0)
			return;


		for (int i = 0; i < SimulateNo; i++)
		{
			auto NewSimulatedHexGraph = &hexGraph;	//Do not clone HexGraph in each simulation run (may cause OOM). Use restore tactic instead. 
			auto winner = runSingleSimulationGame(*NewSimulatedHexGraph, hexGraph.LastMovedPlayer());
			PlayerWinCount[winner]++;
			hexGraph.Restore(HexGraphForRestore); //Restore hexGraph for next run
		}

		if (PlayerWinCount[HexGraph::EnumPlayResult::FirstPlayerWinToken] > PlayerWinCount[HexGraph::EnumPlayResult::SecondPlayerWinToken])
		{
			winner = HexGraph::EnumPlayResult::FirstPlayerWinToken;
			winnerWinCount = PlayerWinCount[HexGraph::EnumPlayResult::FirstPlayerWinToken];
			loser = HexGraph::EnumPlayResult::SecondPlayerWinToken;
			loserWinCount = PlayerWinCount[HexGraph::EnumPlayResult::SecondPlayerWinToken];
		}
		else
		{
			winner = HexGraph::EnumPlayResult::SecondPlayerWinToken;
			winnerWinCount = PlayerWinCount[HexGraph::EnumPlayResult::SecondPlayerWinToken];
			loser = HexGraph::EnumPlayResult::FirstPlayerWinToken;
			loserWinCount = PlayerWinCount[HexGraph::EnumPlayResult::FirstPlayerWinToken];
		}


	}

	//simulate playing until there is a winer
	HexGraph::EnumPlayResult runSingleSimulationGame(HexGraph& stmHxGrph, Node::EnumNodePlayerOrderStatus thePlayer)
	{
		int moveCount = 0;
		HexGraph::EnumPlayResult rslt;
		auto nextMvt = generateRemainedNextMove(stmHxGrph);

		while (true)
		{
			std::string msg, msg2;

			//*********** Get Random Next Move
			auto toPickNode = stmHxGrph.RemainedEmptyNode.begin();
			std::advance(toPickNode, rand() % stmHxGrph.RemainedEmptyNode.size()); //Get Random Next Move
			auto nextMvt = toPickNode->second->showCordinate();
			//***********
			stmHxGrph.place(nextMvt, stmHxGrph.NextMovePlayer(), msg);

			if (stmHxGrph.getPlaceCount() == stmHxGrph.size * stmHxGrph.size)
				return stmHxGrph.JudgeWinner();


		}


	}

	std::string generateRemainedNextMove(HexGraph& hexGraph)
	{

		return drawCandidateMoveNode(hexGraph)->showCordinate();


	}
};