#include<bits/stdc++.h>
#include<algorithm>
#include<sys/time.h>

using namespace std;
#define ll long long
//code



ll gettime(){                //function gettime is used to measure timing of execution
	struct timeval start;
	gettimeofday(&start,NULL);
	return start.tv_sec*1000000+start.tv_usec;
}

typedef struct Node{
	pair<ll, ll> data;
	ll degree;
	struct Node* parent;
	struct Node* child;  
	struct Node* sibling; 	 
}Node;

typedef struct BinomialHeap{
	struct  Node* head;	 
	ll size;
}BinomialHeap;

BinomialHeap* merge_heaps(BinomialHeap* one,BinomialHeap* two){
	BinomialHeap* hdash=(BinomialHeap*)malloc(sizeof(BinomialHeap));
	hdash->head=NULL;
	hdash->size = one->size + two->size;
	vector< pair < ll, Node* > >t1,t2;
	Node* p1=one->head,*p2=two->head,*p3;
	while(p1!=NULL){
		p3=p1;
		p1=p1->sibling;
		p3->sibling=NULL;
		t1.push_back(make_pair(p3->degree,p3));
	}
	while(p2!=NULL){
		p3=p2;
		p2=p2->sibling;
		p3->sibling=NULL;
		t2.push_back(make_pair(p3->degree,p3));
	}
	sort(t1.begin(),t1.end());
	sort(t2.begin(),t2.end());
	ll ch1,ch2;
	vector< pair < ll , Node* > >::iterator itr1,itr2;
	itr1=t1.begin();
	itr2=t2.begin();
	p1=hdash->head;
	while(itr1!=t1.end() && itr2!=t2.end()){
		ch1=itr1->first;
		ch2=itr2->first;
		if(ch1<=ch2){
			if(p1==NULL){
				hdash->head=itr1->second;
				p1=hdash->head;
			}else{
				p1->sibling=itr1->second;
				p1=p1->sibling;
			}
		}else{
			if(p1==NULL){
				hdash->head=itr2->second;
				p1=hdash->head;
			}else{
				p1->sibling=itr2->second;
				p1=p1->sibling;
			}
		}
		itr1++;
		itr2++;
	}
	while(itr1!=t1.end()){
		p1->sibling=itr1->second;
		p1=p1->sibling;
		itr1++;
	}
	while(itr2!=t2.end()){
		p1->sibling=itr2->second;
		p1=p1->sibling;
		itr2++;
	}
	return hdash;
}

BinomialHeap* Binomial_link(BinomialHeap* Bh,Node* y,Node* z){
	y->parent=z;
	y->sibling=z->child;
	z->child=y;
	z->degree++;
	return Bh;
}



BinomialHeap* Union(BinomialHeap* H1,BinomialHeap* H2){
	BinomialHeap* H=(BinomialHeap*)malloc(sizeof(BinomialHeap));
	H=merge_heaps(H1,H2);
	Node* f=H1->head;
	Node* g;
	while(f!=NULL){
		g=f;
		f=f->sibling;
		g->child=NULL;
		g->parent=NULL;
		g->sibling=NULL;
		delete(g);
	}
	f=H2->head;
	while(f!=NULL){
		g=f;
		f=f->sibling;
		g->child=NULL;
		g->parent=NULL;
		g->sibling=NULL;
		delete(g);
	}
	if(H->head==NULL){
		return H;
	}
	Node* prevX=NULL;
	Node* X=H->head;
	Node* nextX=X->sibling;
	while(nextX!=NULL){
		
		if((X->degree!=nextX->degree)||(nextX->sibling!=NULL && nextX->sibling->degree==X->degree)){
			prevX=X;
			X=nextX;
		}else if(X->data.first <= nextX->data.first){
			X->sibling=nextX->sibling;
			H=Binomial_link(H,nextX,X);
		}else if(prevX==NULL){
			H->head=nextX;
		}else{
			prevX->sibling=nextX;
			H=Binomial_link(H,X,nextX);
			X=nextX;
		}
		nextX=X->sibling;
	}
	return H;
}

BinomialHeap* insertion(BinomialHeap* H,pair<ll, ll> key){
	BinomialHeap* hdash=(BinomialHeap*)malloc(sizeof(BinomialHeap));
	Node* x=(Node*)malloc(sizeof(Node));
	x->parent=NULL;
	x->child=NULL;
	x->sibling=NULL;
	x->degree=0;
	x->data=key;
	hdash->head=x;
	hdash->size=1;
	H=Union(H,hdash);
	H->size++;
	return H;
}

Node* getmin(BinomialHeap* H){
	Node* y=NULL;
	Node* x=H->head;
	pair<ll, ll> min;
	min.first=INT_MAX;
	while(x!=NULL){
		if(x->data.first<min.first){
			min=x->data;
			y=x;
		}
		x=x->sibling;
	}
	return y;
}

BinomialHeap* remove_this_Node(Node* tree){
	BinomialHeap* hp=(BinomialHeap*)malloc(sizeof(BinomialHeap));
	Node* temp=tree->child;
	Node* l;
	while(temp!=NULL){
		l=temp;
		temp=temp->sibling;
		l->sibling=NULL;
		if(hp->head==NULL){
			hp->head=l;
		}else{
			l->sibling=hp->head;
			hp->head=l;
		}
	}
	return hp;
}



pair<ll, ll> Extract_min(BinomialHeap* H){
	Node* y=getmin(H);
	pair<ll, ll> rat;
	if(y!=NULL){
		rat=y->data;
	}else{
		rat=make_pair(999999,999999);
	}
	Node* p1=H->head,*p2=H->head,*p3=H->head;
	if(p1==y){
		H->head=p1->sibling;
	}else if(p1->sibling==y){
		p1->sibling=NULL;
	}else{
		if(p2!=NULL){
			p2=p2->sibling;	
		}
		if(p3!=NULL){
			p3=p3->sibling;
			p3=p3->sibling;
		}
		while(p2!=y && p3!=NULL){
			p1=p1->sibling;
			p2=p2->sibling;
			p3=p3->sibling;
		}
		if(p2==y){
			p1->sibling=p2->sibling;
			p2->sibling=NULL;
		}
	}
	BinomialHeap* hdash=(BinomialHeap*)malloc(sizeof(BinomialHeap));
	hdash=remove_this_Node(y);
	H=Union(H,hdash);
	H->size--;
	return rat;
}



int main()           // main function
{
	ll t,proper;
	cin>>t;
	proper=t;
	long double arrtme[t];			//array to store timing of execution of all the testcases
	ll hji=0;
	while(t--){							// loop for running testcases
		ll start = gettime();
		ll n,d,check_ngtv_for_undr=0;
		cin>>n>>d;
		
		vector<ll>adj[n+1];
		for(ll i=0;i<=n;i++){
			adj[i].push_back(999999);
			
		}
		for(ll j=0;j<=n;j++){
			adj[0].push_back(999999);
			
		}
		ll temp;
		for(ll i=1;i<=n;i++){
			for(ll j=1;j<=n;j++){
				
				cin>>temp;
				adj[i].push_back(temp);
				if(i==j){
					adj[i][j]=0;
				}
				if(adj[i][j]<0){
					check_ngtv_for_undr=1;
				}
			}
		}
		if(d==0){				//if it is undirected graph 
			if(check_ngtv_for_undr==1){
				cout<<-1<<"\n";
			}else{
				//dijkstra part
				vector< pair < ll , ll > > adjlit[n+1];			//took a matrix for implementing dijkstra
				for(ll i=1;i<=n;i++){
					for(ll j=1;j<=n;j++){
						if(adj[i][j]!=999999 && i!=j){
							adjlit[i].push_back(make_pair(adj[i][j],j));
						}
					}
				}
				
				
				
				vector<ll>ans[n+1];				//matrix to collect answer
				for(ll i=0;i<=n;i++){
					for(ll j=0;j<=n;j++){
						ans[i].push_back(999999);
					}
					
				}
				for(ll s=1;s<=n;s++){			//applying dijkstra for each vertices
					ll distance[n+1];
					distance[0]=-1;
					for(ll i=1;i<=n;i++){
						distance[i]=999999;
					}
					distance[s]=0;
					bool visited[n+1];
					visited[0]=true;
					for(ll i=1;i<=n;i++){
						visited[i]=false;
					}
					
					BinomialHeap* Bh=(BinomialHeap*)malloc(sizeof(BinomialHeap));		//allocated Binomialheap having capacity n and currentsize 0
					Bh->head=NULL;
					Bh->size=0;
					Bh=insertion(Bh,make_pair(0,s));
					ll sourc,v,w;
					pair < ll , ll > par;
					while(Bh->size!=0){
						par=Extract_min(Bh);		//extracting the min weight
						sourc=par.second;
						if(visited[sourc]==false){
							visited[sourc]=true;
							for(ll j=0;j<adjlit[sourc].size();j++){
								w=adjlit[sourc][j].first;
								v=adjlit[sourc][j].second;
								if(distance[v] > distance[sourc] + w ){		//relaxation
									distance[v]=distance[sourc]+w;
									Bh=insertion(Bh,make_pair(distance[v],v));
								}	
							}							
						}
					}
					for(ll i=1;i<=n;i++){
						ans[s][i]=distance[i];
					}					
				}	
				for(ll i=1;i<=n;i++){
					for(ll j=1;j<=n;j++){
						cout<<ans[i][j]<<" ";
					}
					cout<<"\n";
				}	
			}
		}else{
			ll s=n+1;
			ll total_edges=0;						
			map< pair < ll , ll > , ll >wt;
			vector< pair < ll , ll > > adjlist[n+2];
			
			for(ll i=1;i<=n;i++){
				for(ll j=1;j<=n;j++){
					if(adj[i][j]!=999999 && adj[i][j]!=0){			//if there is no edge betn two vertices and there is self loop then ignore
						adjlist[i].push_back(make_pair(adj[i][j],j));
						wt[make_pair(i,j)]=adj[i][j];		//(weight,vertex)
						total_edges++;
					}
				}
			}
			for(ll i=1;i<=n;i++){								//adding one extra vertex for implementing belman-ford
				adjlist[s].push_back(make_pair(0,i));
				wt[make_pair(s,i)]=0;
				total_edges++;
			}
			
			
			
			ll distance[n+2];							//implemented bellman-ford here
			distance[0]=-1;
			for(ll i=1;i<=n;i++){
				distance[i]=999999;						//marked distances inf 
			}
			distance[s]=0;				//self dis is 0
			bool visit[n+2];
			queue<ll>q;
			visit[0]=true;
			for(ll i=1;i<=n;i++){
				visit[i]=false;
			}
			ll key=total_edges*(n+1),count=0;
			q.push(s);
			visit[s]=true;
			ll f,sec;
			while(!q.empty()){
				f=q.front();
				q.pop();
				visit[f]=false;
				for(ll i=0;i<adjlist[f].size();i++){
					sec=adjlist[f][i].second;
					if(distance[sec]>distance[f]+wt[make_pair(f,sec)]){
						distance[sec]=distance[f]+wt[make_pair(f,sec)];
						if(!visit[sec]){
							q.push(sec);
							visit[sec]=true;
						}
					}	
				}
				count++;
				if(count>key){
					break;
				}
			}
			ll srct=s,flag=0;
			q.push(srct);
			for(ll i=0;i<=n;i++){
				visit[i]=false;
			}
			visit[0]=true;
			visit[srct]=true;
			
			while(!q.empty()){			// to check if there is a negative cycle here
				srct=q.front();
				q.pop();
				for(ll i=0;i<adjlist[srct].size();i++){
					if(distance[adjlist[srct][i].second]>distance[srct]+wt[make_pair(srct,adjlist[srct][i].second)]){
						flag=1;
						break;
					}
					
					
					if(!visit[adjlist[srct][i].second]){
						q.push(adjlist[srct][i].second);
					}
					visit[adjlist[srct][i].second]=true;
			
				}
				if(flag==1){			//if flag is 1then there is a -ve cycle
					break;
				}
				
				
			}
			if(flag==1){
				cout<<-1<<"\n";
			}else{
				for(ll i=1;i<=n;i++){				//reweighting the edges after impementing bellman ford
					for(ll j=1;j<=n;j++){
						if(adj[i][j]==999999 || adj[i][j]==0){
							continue;
						}
						adj[i][j]=adj[i][j]+distance[i]-distance[j];	
					}
				}
				vector< pair < ll , ll > > adjlit[n+1];			//took a matrix for implementing dijkstra
				for(ll i=1;i<=n;i++){
					for(ll j=1;j<=n;j++){
						if(adj[i][j]!=999999 && i!=j){
							adjlit[i].push_back(make_pair(adj[i][j],j));
						}
					}
				}
				
				//dijkstra part
				
				vector<ll>ans[n+1];
				for(ll i=0;i<=n;i++){
					for(ll j=0;j<=n;j++){
						ans[i].push_back(999999);
					}
					
				}
				for(s=1;s<=n;s++){			//for all the vertices we are implementing dijkstra
					ll distanc[n+1];
					distanc[0]=-1;
					for(ll i=1;i<=n;i++){
						distanc[i]=999999;
					}
					distanc[s]=0;
					bool visited[n+1];
					visited[0]=true;
					for(ll i=1;i<=n;i++){
						visited[i]=false;
					}
					BinomialHeap* Bh=(BinomialHeap*)malloc(sizeof(BinomialHeap));		//allocated Binomialheap having capacity n and currentsize 0
					Bh->head=NULL;
					Bh->size=0;
					Bh=insertion(Bh,make_pair(0,s));
					
					ll sourc,v,w;
					pair < ll , ll > par;
					while(Bh->size!=0){
						par=Extract_min(Bh);				//extracting the minimum weight
						sourc=par.second;
						if(visited[sourc]==false){
							visited[sourc]=true;
							for(ll j=0;j<adjlit[sourc].size();j++){
								w=adjlit[sourc][j].first;
								v=adjlit[sourc][j].second;
								if(distanc[v] > distanc[sourc] + w ){			//relaxation
									distanc[v]=distanc[sourc]+w;
									Bh=insertion(Bh,make_pair(distance[v],v));
								}
								
							}
							
							
						}	
					}
					
					
					
					ll yo;
					for(ll i=1;i<=n;i++){							//finallize
						if(distanc[i]==999999){
							yo=distanc[i];
						}else{
							yo=distanc[i]+distance[i]-distance[s];
							
						}
						ans[s][i]=yo;
					}
	
				}	
				
			
				for(ll i=1;i<=n;i++){					//printing the answers
					for(ll j=1;j<=n;j++){
						cout<<ans[i][j]<<" ";
					}
					cout<<"\n";
				}
				
			}
	
		}
		ll end = gettime();
		arrtme[hji]=end - start;
		arrtme[hji]/=1000000;
		hji++;
	}
	for(ll i=0;i<proper;i++){
		cout<<(long double)arrtme[i]<<" ";
	}
	cout<<"\n";
	return 0;
}