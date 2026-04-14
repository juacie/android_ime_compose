#ifndef __Hk2_DATA_UI_H_
#define __Hk2_DATA_UI_H_

#define MAX_Hk2_DATA_UI                    5

struct Hk2_DATA_UI
{
       struct Hk2_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       Hk2_DATA_ID
       DataUI[MAX_Hk2_DATA_UI];
       
       ~Hk2_DATA_UI()
       {
            for ( int i =0; i <MAX_Hk2_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Hk2_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[1 +1];
            DataUI[ 0].WData[ 0] =0x51e3;
            DataUI[ 0].WData[ 1] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[1 +1];
            DataUI[ 1].WData[ 0] =0x37d7;
            DataUI[ 1].WData[ 1] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[1 +1];
            DataUI[ 2].WData[ 0] =0x5e05;
            DataUI[ 2].WData[ 1] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[1 +1];
            DataUI[ 3].WData[ 0] =0x5c1c;
            DataUI[ 3].WData[ 1] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[1 +1];
            DataUI[ 4].WData[ 0] =0x9274;
            DataUI[ 4].WData[ 1] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
