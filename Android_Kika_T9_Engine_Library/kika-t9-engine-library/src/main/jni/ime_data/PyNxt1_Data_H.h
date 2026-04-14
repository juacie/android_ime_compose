#ifndef __PyNxt1_DATA_UI_H_
#define __PyNxt1_DATA_UI_H_

#define MAX_PyNxt1_DATA_UI             6

struct PyNxt1_DATA_UI
{
       struct PyNxt1_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       PyNxt1_DATA_ID
       DataUI[MAX_PyNxt1_DATA_UI];
       
       ~PyNxt1_DATA_UI()
       {
            for ( int i =0; i <MAX_PyNxt1_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       PyNxt1_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[1 +1];
            DataUI[ 0].WData[ 0] =0x5982;
            DataUI[ 0].WData[ 1] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[1 +1];
            DataUI[ 1].WData[ 0] =0x671f;
            DataUI[ 1].WData[ 1] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[1 +1];
            DataUI[ 2].WData[ 0] =0x8bbe;
            DataUI[ 2].WData[ 1] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[1 +1];
            DataUI[ 3].WData[ 0] =0x65e5;
            DataUI[ 3].WData[ 1] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[1 +1];
            DataUI[ 4].WData[ 0] =0x5192;
            DataUI[ 4].WData[ 1] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[1 +1];
            DataUI[ 5].WData[ 0] =0x88c5;
            DataUI[ 5].WData[ 1] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
