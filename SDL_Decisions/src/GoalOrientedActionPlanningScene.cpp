#include "GoalOrientedActionPlanningScene.h"
#include "goap.h"
#include "astar.h"
using namespace std;

GoalOrientedActionPlanningScene::GoalOrientedActionPlanningScene()
{
	pathfinder = aplus; //Modo por defecto

	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze();
	loadTextures("../res/maze.png", "../res/coin.png");
	nodeGrid.InitGrid(terrain); //crea grid

	srand((unsigned int)time(NULL));

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);


	//Setting agent position to middle area
	Vector2D startCell(20,11);
	agents[0]->setPosition(cell2pix(startCell));
	startNodePosition = startCell;
	//PathFollowing next Target
	currentTargetIndex = -1;
	startNewPath = false;

	actionplanner_t ap;
	goap_actionplanner_clear(&ap); // initializes action planner

								   // describe repertoire of actions
	goap_set_pre(&ap, "scout", "armedwithgun", true);
	goap_set_pst(&ap, "scout", "enemyvisible", true);

	goap_set_pre(&ap, "approach", "enemyvisible", true);
	goap_set_pst(&ap, "approach", "nearenemy", true);

	goap_set_pre(&ap, "aim", "enemyvisible", true);
	goap_set_pre(&ap, "aim", "weaponloaded", true);
	goap_set_pst(&ap, "aim", "enemylinedup", true);

	goap_set_pre(&ap, "shoot", "enemylinedup", true);
	goap_set_pst(&ap, "shoot", "enemyalive", false);

	goap_set_pre(&ap, "load", "armedwithgun", true);
	goap_set_pst(&ap, "load", "weaponloaded", true);

	goap_set_pre(&ap, "detonatebomb", "armedwithbomb", true);
	goap_set_pre(&ap, "detonatebomb", "nearenemy", true);
	goap_set_pst(&ap, "detonatebomb", "alive", false);
	goap_set_pst(&ap, "detonatebomb", "enemyalive", false);

	goap_set_pre(&ap, "flee", "enemyvisible", true);
	goap_set_pst(&ap, "flee", "nearenemy", false);

	char desc[4096];
	goap_description(&ap, desc, sizeof(desc));
	LOGI("%s", desc);

	// describe current world state.
	worldstate_t fr;
	goap_worldstate_clear(&fr);
	goap_worldstate_set(&ap, &fr, "enemyvisible", false);
	goap_worldstate_set(&ap, &fr, "armedwithgun", true);
	goap_worldstate_set(&ap, &fr, "weaponloaded", false);
	goap_worldstate_set(&ap, &fr, "enemylinedup", false);
	goap_worldstate_set(&ap, &fr, "enemyalive", true);
	goap_worldstate_set(&ap, &fr, "armedwithbomb", true);
	goap_worldstate_set(&ap, &fr, "nearenemy", false);
	goap_worldstate_set(&ap, &fr, "alive", true);

	goap_set_cost(&ap, "detonatebomb", 5);	// make suicide more expensive than shooting.

	worldstate_t goal;
	goap_worldstate_clear(&goal);
	goap_worldstate_set(&ap, &goal, "enemylinedup", true);
	//goap_worldstate_set( &ap, &goal, "alive", true ); // add this to avoid suicide actions in plan.

	worldstate_t states[16];
	const char* plan[16];
	int plansz = 16;
	const int plancost = astar_plan(&ap, fr, goal, plan, states, &plansz);
	LOGI("plancost = %d", plancost);
	goap_worldstate_description(&ap, &fr, desc, sizeof(desc));
	LOGI("%-23s%s", "", desc);
	for (int i = 0; i<plansz && i<16; ++i)
	{
		goap_worldstate_description(&ap, states + i, desc, sizeof(desc));
		LOGI("%d: %-20s%s", i, plan[i], desc);
	}
	int xd;
}

GoalOrientedActionPlanningScene::~GoalOrientedActionPlanningScene()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void GoalOrientedActionPlanningScene::update(float dtime, SDL_Event *event)
{

	//Dibujar grid si o no:
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	}
	if (startNewPath) { //Encontrar camino mediante aplus
		for (int i = 0; i < agents.size(); i++) {
			cout << "Ejecutando busqueda de camino mediante APLUS modo en ejecucion: " << getCurrentMode((Pathfinders)pathfinder) << endl;
			//path = nodeGrid.APlus(startNodePosition);
			agents[0]->startNewPath = false;
			startNewPath = false;
		}
	}

	//EJECUTAR LA TRAYECTORIA A TRAV�S DEL CAMINO:
	if ((currentTargetIndex == -1) && (path.points.size()>0))
		currentTargetIndex = 0;

	if (currentTargetIndex >= 0)
	{
		float dist = Vector2D::Distance(agents[0]->getPosition(), path.points[currentTargetIndex]);
		if (dist < path.ARRIVAL_DISTANCE)
		{

			if (currentTargetIndex == path.points.size() - 1)
			{
				if (dist < 3)
				{
					path.points.clear();
					nodeGrid.Clear(); //resets grid
					currentTargetIndex = -1;
					agents[0]->setVelocity(Vector2D(0, 0));
					// if we have arrived to the coin, replace it in a random cell!
					//Nuevo target y nuevo path:
					startNodePosition = pix2cell(agents[0]->getPosition());
					nodeGrid.NewTarget(agents[0]->nextTarget); //Cambia el objetivo del pathfinder
					
				}
				else
				{
					Vector2D steering_force = agents[0]->Behavior()->Arrive(agents[0], currentTarget, path.ARRIVAL_DISTANCE, dtime);
					agents[0]->update(steering_force, dtime, event);
				}
				return;
			}
			currentTargetIndex++;
		}

		currentTarget = path.points[currentTargetIndex];
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);
	}
	else
	{
		agents[0]->update(Vector2D(0, 0), dtime, event);
	}
	//seteamos primer path
	if (firstPath) {
		path.points.clear();
		nodeGrid.Clear(); //resets grid
		currentTargetIndex = -1;
		agents[0]->setVelocity(Vector2D(0, 0));
		// if we have arrived to the coin, replace it in a random cell!
		//Nuevo target y nuevo path:
		startNodePosition = pix2cell(agents[0]->getPosition());
		startNewPath = true;
		nodeGrid.NewTarget(agents[0]->nextTarget); //Cambia el objetivo del pathfinder
		firstPath = false;
	}
	
	//no se por k pero sin esto no funciona jaja
	if (agents[0]->startNewPath) {
		startNewPath = true;
		path.points.clear();
		nodeGrid.Clear(); //resets grid
		currentTargetIndex = -1;
		agents[0]->setVelocity(Vector2D(0, 0));
		// if we have arrived to the coin, replace it in a random cell!
		//Nuevo target y nuevo path:
		startNodePosition = pix2cell(agents[0]->getPosition());
		nodeGrid.NewTarget(agents[0]->nextTarget); //Cambia el objetivo del pathfinder
		agents[0]->startNewPath = false;
	}
	
}

void GoalOrientedActionPlanningScene::draw()
{
	drawMaze();
	drawCoin();


	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	for (int i = 0; i < (int)path.points.size(); i++)
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (i > 0)
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
	}

	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);

	agents[0]->draw();
}

const char* GoalOrientedActionPlanningScene::getTitle()
{
	return "SDL Steering Behaviors :: PathFinding1 Demo";
}

void GoalOrientedActionPlanningScene::drawMaze()
{
	if (draw_grid)
	{

		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else
	{
		//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
	}
}

void GoalOrientedActionPlanningScene::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void GoalOrientedActionPlanningScene::initMaze()
{

	// Initialize a list of Rectagles describing the maze geometry (useful for collision avoidance)
	SDL_Rect rect = { 0, 0, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 128, 160, 1152, 32 };
	maze_rects.push_back(rect);
	rect = { 0, 736, 1280, 32 };
	maze_rects.push_back(rect);
	
	rect = { 0,32,32,736 };
	maze_rects.push_back(rect);
	rect = { 1248,32,32,736 };
	maze_rects.push_back(rect);

	rect = { 416,512,32,268 };
	maze_rects.push_back(rect);
	rect = { 832,512,32,268 };
	maze_rects.push_back(rect);
	
	rect = { 32,480,128,32 };
	maze_rects.push_back(rect);
	rect = { 288,480,288,32 };
	maze_rects.push_back(rect);
	rect = { 704,480,288,32 };
	maze_rects.push_back(rect);
	rect = { 1120,480,128,32 };
	maze_rects.push_back(rect);

	// Initialize the terrain matrix (for each cell a zero value indicates it's a wall)
	
	// (1st) initialize all cells to 1 by default
	for (int i = 0; i < num_cell_x; i++)
	{
		vector<int> terrain_col(num_cell_y, 1); 
		terrain.push_back(terrain_col);
	}
	// (2nd) set to zero all cells that belong to a wall
	int offset = CELL_SIZE / 2;
	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			Vector2D cell_center ((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset));
			for (unsigned int b = 0; b < maze_rects.size(); b++)
			{
				if (Vector2DUtils::IsInsideRect(cell_center, (float)maze_rects[b].x, (float)maze_rects[b].y, (float)maze_rects[b].w, (float)maze_rects[b].h))
				{
					terrain[i][j] = 0;
				    break;
				}  
			}
			
		}
	}

}

bool GoalOrientedActionPlanningScene::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D GoalOrientedActionPlanningScene::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D GoalOrientedActionPlanningScene::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x/CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool GoalOrientedActionPlanningScene::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()) )
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}