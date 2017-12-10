#include "Player.hh"
#include <map>
#include<queue>
#include<list>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Drea


struct PLAYER_NAME : public Player {

    /**
    * Factory: returns a new instance of this class.
    * Do not modify this function.
    */
    static Player* factory () {
    return new PLAYER_NAME;
    }
    
    map <int, City> cities;
    map <int, Path> paths;
    vector <vector <int> > count_c;
    vector <vector <int> > count_p;

    vector <Pos> fpos; //posició on han d'anar els MEUS orks, si es -1, es d'altres players
    vector <queue <Dir> > ways; //cua amb les direccions que han de seguir calculat pel bfs
    typedef vector < vector<bool> > matrix_bool;

    
    //Find cities and paths of the board
    void find_c_p () {
        Cell c;
        
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                c = cell(i,j);
                if (c.type == CITY) {
                    if (cities.find(c.city_id) == cities.end()) 
                        cities.insert({c.city_id, city(c.city_id)});
                    
                }

                else if (c.type == PATH) {
                    if (paths.find(c.path_id) == paths.end()) 
                        paths.insert({c.path_id, path(c.path_id)});
                    
                }
            }
        }
    }
    
    

    void bfs(const Pos &upos, Pos &fp, queue<Dir> &qq){
        matrix_bool visited(rows(), vector <bool> (cols(), false));
        queue<pair<Pos, queue<Dir> > > q;
        q.push({upos, queue <Dir>()});
        bool found = false;
        visited[upos.i][upos.j] = true;

        while (not q.empty() and not found){
            pair <Pos, queue <Dir> > p = q.front();
            q.pop();

            for (int i = 0; i < 4 and not found; ++i){
                queue<Dir> q2 = p.second;
                Pos pd = p.first + Dir(i);

                if (not visited[pd.i][pd.j]){
                    Cell c = cell(pd);

                    if (c.type != WATER){
                        q2.push(Dir(i));
                        q.push({pd, q2});
                        visited[pd.i][pd.j] = true;

                        if (c.type == CITY or c.type == PATH){
                            found = true;
                            fp = pd;
                            qq = q2;
                        }
                    }
                }
            }
        }        
    }
/*
    void cities_paths_un () {
        map<int,City>::iterator itc;
        map<int,Path>::iterator itp;
        itc = cities.begin();
        itp = paths.begin();
        int count_c = 0;
        vector <int> city_count (nb_cities());
        while(itc != cities.end()){

        }
    }*/
    double distancia (const Pos & ap, const Pos & fp) {
        return sqrt(((ap.i + fp.i)^2) + ((ap.j + fp.j)^2));
    }
    /**
    * Types and attributes for your player can be defined here.
    */

    /**
    * Play method, invoked once per each round.
    */
    //vector <int> last_round;

    virtual void play () {
        vector <int> my_orks = orks(me());

        //buscar ciutats
        if (round() == 0) {
            find_c_p();
            fpos = vector<Pos> (nb_units(),{0,0});
            ways = vector <queue <Dir> > (nb_units());
            count_c = vector <vector <int> > (nb_cities());
            count_p = vector <vector <int> > (nb_paths());
            vector <Pos> ccc = paths[0].second;
            cerr << "Path 0: ";
            for(int i = 0; i < int(ccc.size()); ++i) {
                cerr << ccc[i] << ' ';
            }


            for (int id_ork : my_orks){
                Unit u = unit(id_ork);
                bfs(u.pos, fpos[u.id], ways[u.id]);
            }    
            
        }

        if (round() != 0) {
            
            for (int i = 0; i < int(my_orks.size()); ++i) {
                Unit u = unit(my_orks[i]);
                if (u.health == 100) bfs (u.pos, fpos[u.id], ways[u.id]);
            }
        }

       for (int id_ork : my_orks){

            if (not ways[id_ork].empty()){;
                execute(Command(id_ork, ways[id_ork].front()));
                ways[id_ork].pop();
            }
            else {
                Unit u = unit (id_ork);
                Cell c = cell(u.pos);
                if (c.type == CITY) count_c[c.city_id].push_back(id_ork);
                if (c.type == PATH) count_p[c.path_id].push_back(id_ork);
            }
        }

        /*
        for (int i = 0; i < nb_cities; ++i) {
            for(int j = 1; j < int(count_c[i].size()); ++j)
                if(count_c[i].size() > 1) { //hi ha mes de un orco
                    int id_move = count_c[i][j];

                }
        }


        for(int i = 0; i < nb_paths(); ++i) {
            for(int j = 1; j < int(count_c[i].size()); ++j){
                int id_move = count_p[i][j];
                Unit move = unit(id_move);
                int a = distancia()
            }
            
        }*/
        cerr << "cpu using: " << status(me()) << endl;
        

        
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
