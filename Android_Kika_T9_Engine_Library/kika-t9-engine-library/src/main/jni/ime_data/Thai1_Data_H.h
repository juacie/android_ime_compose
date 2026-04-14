#ifndef __Thai1_DATA_UI_H_
#define __Thai1_DATA_UI_H_

#define MAX_Thai1_DATA_UI              15

struct Thai1_DATA_UI
{
       struct Thai1_DATA_ID
       {
            wchar_t*          WData;
            wchar_t*          OData;
       };
       
       Thai1_DATA_ID
       DataUI[MAX_Thai1_DATA_UI];
       
       ~Thai1_DATA_UI()
       {
            for ( int i =0; i <MAX_Thai1_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Thai1_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[5 +1];
            DataUI[ 0].WData[ 0] =0xe22;
            DataUI[ 0].WData[ 1] =0xe35;
            DataUI[ 0].WData[ 2] =0xe19;
            DataUI[ 0].WData[ 3] =0xe2a;
            DataUI[ 0].WData[ 4] =0xe4c;
            DataUI[ 0].WData[ 5] ='\0';

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[5 +1];
            DataUI[ 1].WData[ 0] =0xe21;
            DataUI[ 1].WData[ 1] =0xe31;
            DataUI[ 1].WData[ 2] =0xe19;
            DataUI[ 1].WData[ 3] =0xe2a;
            DataUI[ 1].WData[ 4] =0xe4c;
            DataUI[ 1].WData[ 5] ='\0';

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[5 +1];
            DataUI[ 2].WData[ 0] =0xe22;
            DataUI[ 2].WData[ 1] =0xe31;
            DataUI[ 2].WData[ 2] =0xe19;
            DataUI[ 2].WData[ 3] =0xe2e;
            DataUI[ 2].WData[ 4] =0xe35;
            DataUI[ 2].WData[ 5] ='\0';

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[5 +1];
            DataUI[ 3].WData[ 0] =0xe1f;
            DataUI[ 3].WData[ 1] =0xe34;
            DataUI[ 3].WData[ 2] =0xe19;
            DataUI[ 3].WData[ 3] =0xe2a;
            DataUI[ 3].WData[ 4] =0xe4c;
            DataUI[ 3].WData[ 5] ='\0';

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[5 +1];
            DataUI[ 4].WData[ 0] =0xe22;
            DataUI[ 4].WData[ 1] =0xe34;
            DataUI[ 4].WData[ 2] =0xe19;
            DataUI[ 4].WData[ 3] =0xe2a;
            DataUI[ 4].WData[ 4] =0xe4c;
            DataUI[ 4].WData[ 5] ='\0';

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[5 +1];
            DataUI[ 5].WData[ 0] =0xe21;
            DataUI[ 5].WData[ 1] =0xe34;
            DataUI[ 5].WData[ 2] =0xe19;
            DataUI[ 5].WData[ 3] =0xe2b;
            DataUI[ 5].WData[ 4] =0xe4c;
            DataUI[ 5].WData[ 5] ='\0';

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[6 +1];
            DataUI[ 6].WData[ 0] =0xe21;
            DataUI[ 6].WData[ 1] =0xe31;
            DataUI[ 6].WData[ 2] =0xe19;
            DataUI[ 6].WData[ 3] =0xe2d;
            DataUI[ 6].WData[ 4] =0xe48;
            DataUI[ 6].WData[ 5] =0xe30;
            DataUI[ 6].WData[ 6] ='\0';

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[6 +1];
            DataUI[ 7].WData[ 0] =0xe21;
            DataUI[ 7].WData[ 1] =0xe31;
            DataUI[ 7].WData[ 2] =0xe19;
            DataUI[ 7].WData[ 3] =0xe2d;
            DataUI[ 7].WData[ 4] =0xe49;
            DataUI[ 7].WData[ 5] =0xe30;
            DataUI[ 7].WData[ 6] ='\0';

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[6 +1];
            DataUI[ 8].WData[ 0] =0xe21;
            DataUI[ 8].WData[ 1] =0xe31;
            DataUI[ 8].WData[ 2] =0xe19;
            DataUI[ 8].WData[ 3] =0xe2d;
            DataUI[ 8].WData[ 4] =0xe48;
            DataUI[ 8].WData[ 5] =0xe2d;
            DataUI[ 8].WData[ 6] ='\0';

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[6 +1];
            DataUI[ 9].WData[ 0] =0xe1f;
            DataUI[ 9].WData[ 1] =0xe34;
            DataUI[ 9].WData[ 2] =0xe19;
            DataUI[ 9].WData[ 3] =0xe2a;
            DataUI[ 9].WData[ 4] =0xe38;
            DataUI[ 9].WData[ 5] =0xe14;
            DataUI[ 9].WData[ 6] ='\0';

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[6 +1];
            DataUI[10].WData[ 0] =0xe21;
            DataUI[10].WData[ 1] =0xe31;
            DataUI[10].WData[ 2] =0xe19;
            DataUI[10].WData[ 3] =0xe2a;
            DataUI[10].WData[ 4] =0xe4c;
            DataUI[10].WData[ 5] =0xe46;
            DataUI[10].WData[ 6] ='\0';

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[8 +1];
            DataUI[11].WData[ 0] =0xe21;
            DataUI[11].WData[ 1] =0xe31;
            DataUI[11].WData[ 2] =0xe19;
            DataUI[11].WData[ 3] =0xe2a;
            DataUI[11].WData[ 4] =0xe4c;
            DataUI[11].WData[ 5] =0xe21;
            DataUI[11].WData[ 6] =0xe32;
            DataUI[11].WData[ 7] =0xe01;
            DataUI[11].WData[ 8] ='\0';

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[6 +1];
            DataUI[12].WData[ 0] =0xe1e;
            DataUI[12].WData[ 1] =0xe31;
            DataUI[12].WData[ 2] =0xe19;
            DataUI[12].WData[ 3] =0xe2d;
            DataUI[12].WData[ 4] =0xe48;
            DataUI[12].WData[ 5] =0xe30;
            DataUI[12].WData[ 6] ='\0';

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[6 +1];
            DataUI[13].WData[ 0] =0xe1f;
            DataUI[13].WData[ 1] =0xe31;
            DataUI[13].WData[ 2] =0xe19;
            DataUI[13].WData[ 3] =0xe2d;
            DataUI[13].WData[ 4] =0xe48;
            DataUI[13].WData[ 5] =0xe30;
            DataUI[13].WData[ 6] ='\0';

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[7 +1];
            DataUI[14].WData[ 0] =0xe21;
            DataUI[14].WData[ 1] =0xe31;
            DataUI[14].WData[ 2] =0xe19;
            DataUI[14].WData[ 3] =0xe2a;
            DataUI[14].WData[ 4] =0xe4c;
            DataUI[14].WData[ 5] =0xe14;
            DataUI[14].WData[ 6] =0xe35;
            DataUI[14].WData[ 7] ='\0';

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
