#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

class LcgGenerator{
private:
    uint16_t mA=0,mC=0,mCurrentX=0,mM=0;
public:
    LcgGenerator(uint16_t a, uint16_t c, uint16_t curent, uint16_t m){
        mA=a;
        mC=c;
        mCurrentX=curent;
        mM=m;
    }

    uint16_t next(){
        mCurrentX=(mA*mCurrentX+mC)%mM;
        return mCurrentX;
    }

};


class Card{
private:
    string mSuite="";
    uint16_t mValue=0;

public:
    Card(const string tipcarte,uint16_t valoare){
        mSuite=tipcarte;
        mValue=valoare;
    }

    bool operator ==(const Card &obiect)const{
        if(obiect.mSuite==mSuite && obiect.mValue==mValue)
            return true;
        else
        {
            return false;
        }
    }

    string toString()const
    {
        return (to_string(mValue)+","+mSuite);

    }



    bool isFake()const
    {
        if(mValue<2 || mValue>14 || mSuite!="dimes" &&  mSuite!="red" && mSuite!="black" && mSuite!="clubs")
            return true;
        else
        {
            return false;
        }
    }

    uint16_t getBlackjackValue()const
    {   uint16_t puncte=0;
        if(mValue<=10){
            puncte=mValue;}
        else

        {
            puncte=10;

        }

        return puncte;

    }


    string get_Msuite(){
        return mSuite;
    }

    uint16_t get_value(){
        return mValue;
    }

};

class CardPack{
private:
    vector <Card> mCards;
    uint16_t mCurrentIndex=0;

public:

    CardPack(bool empty=true) {
        uint16_t i=0,j=0;
        if(empty==true){
            mCards.clear();
            mCurrentIndex=0;
        }
        else
        {
            for(i=0;i<4;i++)
            {
                for(j=2;j<=14;j++)
                {
                    if(j!=11)
                    {
                        if(i==0)
                            mCards.push_back(Card("red",j));

                        if(i==1)
                            mCards.push_back( Card("black",j));


                        if(i==2)
                            mCards.push_back(Card("dimes",j));

                        if(i==3)
                            mCards.push_back( Card("clubs",j));
                    }
                }
            }
            mCurrentIndex=0;
        }

    }

    bool goodForBackjack()const
    {
        for(int i=0;i<mCards.size();i++)
        {
            if(mCards[i].isFake())
                return false;
        }

        return true;

    }


    bool ok()const
    { int cont=0,bun=0,cons=0,final=0;

        for(int i=0;i<mCards.size();i++)
        {
            if(mCards[i].isFake()==true)
                cont=1;
        }


        if(mCards.size()==52)
        {
            bun=1;

        }


        for(int i=0;i<mCards.size()-1;i++)
            for(int j=0;j<mCards.size();j++)
            {
                if(mCards[i]==mCards[i+1])
                    cons=1;
            }


        if(cont==0 && bun==1 && cons==0)
            return true;

        else
        {
            return false;
        }
    }


    void addCard(const Card &carte){
        mCards.push_back(carte);
    }

    void shuffle(uint16_t a1, uint16_t c1,uint16_t x01,uint16_t a2, uint16_t c2, uint16_t x02){
        uint16_t ind0=0,ind1=0;
        mCurrentIndex=0;
        LcgGenerator G0(a1, c1,x01, mCards.size());
        LcgGenerator G1(a2,c2,x02, mCards.size());
        for(int i=0;i<50;i++){
            ind0=G0.next();
            ind1=G1.next();
            swap(mCards[ind0],mCards[ind1]);
        }
    }

    void printPack()const{
        for(int i=0;i<mCards.size();i++){
            cout << mCards[i].toString()<<endl;
        }
    }

    Card nextCard(){
        mCurrentIndex++;
        return mCards[mCurrentIndex-1];


    }

};

class Player{
public:
    string mName="";
    uint16_t mMoney=0;
    uint16_t mPoints=0;

    Player(string nume, uint16_t bani)
    {
        mName=nume;
        mMoney=bani;
        mPoints=0;
    }

    string toString()const
    {
        return (mName+": "+to_string(mMoney));

    }

    void pick(const Card &puncte){
        mPoints=mPoints+puncte.getBlackjackValue();
    }

    void play(CardPack &joaca){
        while(mPoints<17){
            pick(joaca.nextCard());
        }
    }

    void newRound(){
        mPoints=0;
    }

    bool lostRound()const{
        if(mPoints>21)
            return true;
        else
        {
            return false;
        }
    }

    uint16_t getPoints()const{
        return mPoints;
    }

    void  operator --(){
        mMoney=mMoney-10;

    }

    void operator ++(){
        mMoney=mMoney+10;

    }

    bool lostGame()const{
        if(mMoney<10)
            return true;
        else
        {
            return false;
        }

    }
};



int main()
{
    string command="";
    string stema_carte="";

    int cont=0;
    cin>>command;

    if(command=="check_cards")
    {
        CardPack pachet(true);
        uint16_t numar_carte=0;
        while(cin>>numar_carte)
        {
            getchar();
            cin>>stema_carte;
            Card carte(stema_carte,numar_carte);
            pachet.addCard(carte);
        }

        if(pachet.goodForBackjack())
        {
            if(pachet.ok())
                cout<<"Pack OK\n";
            else
                cout<<"Ready for Blackjack\n";
        }
        else
            cout<<"Ragged Pack\n";
    }

    uint16_t a0=0,c0=0,X0=0,a1=0,c1=0,X1=0;

    if(command=="shuffle_cards")
    {
        CardPack pachet(true);
        cin>>a0>>c0>>X0>>a1>>c1>>X1;
        uint16_t numar_carte=0;
        while(cin>>numar_carte)
        {
            getchar();
            cin>>stema_carte;
            Card carte(stema_carte,numar_carte);
            pachet.addCard(carte);
        }


        pachet.shuffle(a0,c0,X0,a1,c1,X1);
        pachet.printPack();


    }

    uint16_t numarjucatori=0;
    string numejucator="";
    uint16_t bani=0;
    vector <Player> jucatori;
    Player dealer("dealer",9999);
    if(command=="play_game")
    {
        CardPack pachet(false);
        cin>>numarjucatori;
        for(int i=0;i<numarjucatori;i++)
        {
            cin>>numejucator;
            cin>>bani;
            Player jucator(numejucator,bani);
            jucatori.push_back(jucator);

        }
        uint16_t a0=0,c0=0,x0=0,a1=0,c1=0,x1=0;
        while(cin>>a0>>c0>>x0>>a1>>c1>>x1)
        {
            pachet.shuffle(a0,c0,x0,a1,c1,x1);

            for(int i=0;i<jucatori.size();i++){
                jucatori[i].pick(pachet.nextCard());
                jucatori[i].pick(pachet.nextCard());
            }

            dealer.pick(pachet.nextCard());
            dealer.pick(pachet.nextCard());

            for(int i=0;i<jucatori.size();i++){
                jucatori[i].play(pachet);
            }

            dealer.play(pachet);

            for(int i=0;i<jucatori.size();i++)
            {
                if(jucatori[i].lostRound()==true ||
                   (dealer.lostRound()!=true && dealer.getPoints()>jucatori[i].getPoints()) )
                {
                    --jucatori[i];
                }

                if(jucatori[i].lostRound()!=true && (dealer.getPoints()<jucatori[i].getPoints() || dealer.lostRound()==true))
                {
                    ++jucatori[i];
                }

                if(jucatori[i].lostGame()!=true)
                {
                    jucatori[i].newRound();

                }
                else
                {
                    jucatori.erase(jucatori.begin()+i);
                    i--;
                }
            }

            dealer.newRound();
        }


        for(int i=0;i<jucatori.size();i++){
            cout<<jucatori[i].toString()<<endl;
        }

    }



    return 0;
}
