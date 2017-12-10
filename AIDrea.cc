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
    
    typedef vector <int> node;
    typedef vector <node> graph;

    vector <Pos> fpos; //posició on han d'anar els MEUS orks, si es -1, es d'altres players
    vector <queue <Pos> > ways; //cua amb les direccions que han de seguir calculat pel bfs
    typedef vector < vector<bool> > matrix_bool;

    void move (int id_ork, const Pos & fp) {
        Unit ork = unit (id_ork);
        Dir next = NONE;
        if (fp.i < ork.pos.i ) next = TOP;
        if (fp.i > ork.pos.i) next = BOTTOM;
        if (fp.j < ork.pos.j) next = LEFT;
        if (fp.j > ork.pos.j) next = RIGHT;
        //cerr << next << endl;
        if (dir_ok(next)) {
            ork.pos += next;
            if (pos_ok(ork.pos)) execute(Command(id_ork, next));
        }
        return;
        
    }
    void move2 (int id_ork, const Pos & fp) {
        Unit ork = unit (id_ork);
        Dir next = NONE;
        if (ork.pos.i < fp.i) next = BOTTOM;
        else if(ork.pos.i > fp.i) next = TOP;
        else if (ork.pos.j < fp.j) next = RIGHT;
        else if (ork.pos.j > fp.j) next = LEFT;
        //cerr << next << endl;
        if (next != NONE and dir_ok(next)) {
            ork.pos += next;
            if (pos_ok(ork.pos)) execute(Command(id_ork, next));
        }
        return;
        
    }
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
    
    

    void bfs (const Pos & upos, Pos & fp, queue<Pos> & qq) {
        //cerr << "inici bfs" << endl;
        matrix_bool visited(rows(), vector <bool> (cols(), false));
        
        queue <pair<Pos, queue <Pos> > > q;

        pair <Pos, queue <Pos> > p = {upos, queue <Pos>()};
        //p.second.push(NONE);
        q.push(p);
        bool found = false;
        visited[p.first.i][p.first.j] = true;
        
        while (not q.empty() and not found) {
            p = q.front();
            q.pop();
            
            
            /*if (not visited[p.first.i][p.first.j]) {
                visited[p.first.i][p.first.j] = true;*/
                
                for (int i = 0; i < 4 and not found; ++i) {                    
                    

                    Pos pd = p.first + Dir(i);
                    
                    
                    if (not visited[pd.i][pd.j]) {
                        Cell c = cell(pd);
                        
                        
                        if (c.type != WATER) {
                            p.second.push(p.first);

                            p.first = pd;
                            
                            q.push(p);
                            visited[pd.i][pd.j] = true;
                            if (c.type == CITY or c.type == PATH) {
                                found = true;
                                fp = pd;
                                qq = p.second;

                        }
                    }
                }
            
        }

        
     }   //cerr << "final bfs" << endl;
    }

    void cities_paths_un () {
        map<int,City>::iterator itc;
        map<int,Path>::iterator itp;
        itc = cities.begin();
        itp = paths.begin();
        int count_c = 0;

        while(itc != cities.end()){
            
        }
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
        /*for (int i = 0; i < int(my_orks.size()); ++i) {
            Unit u = unit (my_orks[i])
            move (my_orks[i]);
        }*/
        //buscar ciutats
        if (round() == 0) {
            find_c_p();
            fpos = vector<Pos> (nb_units(),{0,0});
            ways = vector <queue <Pos> > (nb_units());

            /*map<int, City>::iterator itc;
            itc = cities.begin();
            map<int, Path>::iterator itp = paths.begin();
            for (itc = cities.begin(); itc != cities.end(); ++itc) {
                cerr << "City with id: " << (*itc).first << " at " << itc->second[1] << endl;
            }
            for (itp = paths.begin(); itp != paths.end(); ++itp) {
                cerr << "Path with id: " << (*itp).first;
                cerr << " connects " << (*itp).second.first.first << " and " << (*itp).second.first.second << endl;
             }*/
            cerr <<"inici busqueda "; 
            for (int i = 0; i < int(my_orks.size()); ++i) {
                
                Unit u = unit(my_orks[i]);
        
                bfs(u.pos, fpos[u.id], ways[u.id]);
                cerr << u.id << " ha trobat ciutat o cami" << endl;
                
            
            }
            //cerr << "Ork " << my_orks[0] << " must do this way: ";
            /*for (queue<Dir>::iterator it = ways[0].begin(); it != ways.end(); it++) {
                cerr << *it << ' ';

            }*/
            cerr << my_orks[0] << " ha de fer la ruta: ";
            queue<Pos> example = ways[my_orks[0]];
            while(not example.empty())
             {
                cerr << example.front() << ' ';
                example.pop();
            }
            cerr << endl;
            
            
            /*cerr << "cami a seguir per l'ork " << o.id << " a la pos "<< o.pos << " es:";
            for(list<Pos>::iterator it = way.begin(); it != way.end(); ++it) {
                cerr << *it << ' ';
            }
            cerr << endl;*/
            for (int i = 0; i < int(my_orks.size()); ++i) {
                Unit u = unit(my_orks[i]);
                Cell cc = cell (fpos[u.id]);
                cerr << "ork at " << u.pos << "have to go to: " << fpos[u.id] << "which is a " << cc.type << endl;
                }    
            
        }
        
        if (round() != 0) {
            
            for (int i = 0; i < int(my_orks.size()); ++i) {
                Unit u = unit(my_orks[i]);
                if (u.health == 100) bfs (u.pos, fpos[u.id], ways[u.id]);
            }
        }


        /*for (int i = 0; i < int(my_orks.size()); ++i) {
            Unit u = unit(my_orks[i]);
            Cell c = cell(u.pos);
            if (c == CITY) {


            }
        }*/

        /*
        for (int i = 0; i < int(my_orks.size()); ++i) {
            Unit u = unit (my_orks[i]);
            move2(u.id, fpos[u.id]);
        }*/
        cerr << "my orks are :" << endl;
        for(int i = 0; i < int(my_orks.size()); ++i) {
            int id_ork = my_orks[i];
            cerr << id_ork << ' ';
            Unit u = unit (id_ork);

            if (fpos[id_ork] != u.pos){
                //Dir dd = p_to_d(ways[id_ork].front(), u.pos);
                cerr << ways[id_ork].front() << ' ' << u.pos ;
                
                //cerr << "direction of " << id_ork << " is " << dd;
                
                move(id_ork, ways[id_ork].front());
                ways[id_ork].pop();
                cerr << " done";   
            }
            if (my_orks.size() != 0) {
                u = unit(id_ork);
                cerr << "and it's at " << u.pos  << " with health " << u.health << endl;
            }
            else cerr << "All my orks died" << endl;
        }
        cerr << "cpu using: " << status(me()) << endl;
        

        
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
