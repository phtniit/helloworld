#pragma comment(linker, "/STACK:102400000,102400000")
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;

const int maxn = 110000 ;

int up[maxn],dn[maxn];
int ru[maxn],rd[maxn];
int in[maxn],de[maxn];
int f[maxn],w[maxn],dep[maxn];

void fin(int x)
{
	if(f[x]==x)    return;
	fin(f[x]);
	if(f[x]==f[f[x]])    return;
	int y=f[x];    f[x]=f[f[x]];

	in[x]=max(in[x],in[y]);
	in[x]=max(in[x],up[y]+ru[x]-1);

	if(ru[y]==dep[y]-dep[f[y]]+1)
		ru[x]=ru[x]+ru[y]-1;
	else
		ru[x]=ru[y];
	if(up[x]==dep[x]-dep[y]+1)    up[x]=up[y]+dep[x]-dep[y];

	de[x]=max(de[x],de[y]);
	de[x]=max(de[x],dn[y]+rd[x]-1);
	if(rd[y]==dep[y]-dep[f[y]]+1)
		rd[x]=rd[x]+rd[y]-1;
	else
		rd[x]=rd[y];
	if(dn[x]==dep[x]-dep[y]+1)    dn[x]=dn[y]+dep[x]-dep[y];
}
void adj(int x,int y)
{
	f[x]=y;
	if(w[x]<w[y])
		up[x]=ru[x]=in[x]=2;
	if(w[x]>w[y])
		dn[x]=rd[x]=de[x]=2;
}

int res[maxn];
vector<int>q[maxn];
vector<int>g[maxn];
void dfs(int x)
{
	f[x]=x;
	up[x]=ru[x]=in[x]=1;
	dn[x]=rd[x]=de[x]=1;
	for(int i=0;i<g[x].size();++i)
	{
		int y=g[x][i];
		dfs(y);
		adj(y,x);
	}
	for(int i=0;i<q[x].size();i+=3)
	{
		int u=q[x][i],v=q[x][i+1],idx=q[x][i+2];
		fin(u);    fin(v);
		res[idx]=max(res[idx],in[u]);
		res[idx]=max(res[idx],de[v]);
		res[idx]=max(res[idx],ru[u]+rd[v]-1);
	}
}

int uu[maxn],vv[maxn],lca[maxn];
int fa[maxn],col[maxn];
vector<int>gg[maxn];
int find(int x){
	return fa[x]==x?x:find(fa[x]);
}
void down(int x)
{
	fa[x]=x;
	for(int i=0;i<g[x].size();++i)
	{
		int y=g[x][i];
		dep[y]=dep[x]+1;
		down(y);
		fa[y]=x;
	}
	col[x]=1;
	for(int i=0;i<gg[x].size();i+=2)if(col[gg[x][i]])
		lca[gg[x][i+1]]=find(gg[x][i]);
}

int main()
{
	int tes,n,m;
	scanf("%d",&tes);
	for(int h=1;h<=tes;++h)
	{
		scanf("%d",&n);
		for(int i=1;i<=n;++i)
		{
			g[i].clear();
			q[i].clear();
			gg[i].clear();
			col[i]=0;
			scanf("%d",&w[i]);
		}
		for(int i=2;i<=n;++i)
			scanf("%d",&fa[i]),g[fa[i]].push_back(i);
		scanf("%d",&m);
		for(int u,v,i=0;i<m;++i)
		{
			scanf("%d %d",&u,&v);
			res[i]=0;
			uu[i]=u; vv[i]=v;
			gg[u].push_back(v);
			gg[u].push_back(i);
			gg[v].push_back(u);
			gg[v].push_back(i);
		}
		dep[1]=0;    down(1);
		for(int i=0;i<m;++i)
		{
			q[lca[i]].push_back(uu[i]);
			q[lca[i]].push_back(vv[i]);
			q[lca[i]].push_back(i);
		}
		dfs(1);
		printf("Case #%d:\n",h);
		for(int i=0;i<m;++i)    printf("%d\n",res[i]);
		if(h!=tes)    puts("");
	}
	return 0;
}
