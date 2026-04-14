#ifndef __key_korean_data_ID_H_
#define __key_korean_data_ID_H_

#define MAX_key_korean_data_ID                    23

struct key_korean_data_ID
{
       struct key_korean_data_UI
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       key_korean_data_UI
       DataUI[MAX_key_korean_data_ID];
       
       ~key_korean_data_ID()
       {
            for ( int i =0; i <MAX_key_korean_data_ID; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       key_korean_data_ID()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[3 +1];
            DataUI[ 0].WData[ 0] =0x3163;
            DataUI[ 0].WData[ 1] =0x2c;
            DataUI[ 0].WData[ 2] =0x31;
            DataUI[ 0].WData[ 3] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[3 +1];
            DataUI[ 1].WData[ 0] =0x318d;
            DataUI[ 1].WData[ 1] =0x2c;
            DataUI[ 1].WData[ 2] =0x32;
            DataUI[ 1].WData[ 3] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[4 +1];
            DataUI[ 2].WData[ 0] =0xff1a;
            DataUI[ 2].WData[ 1] =0x2c;
            DataUI[ 2].WData[ 2] =0x32;
            DataUI[ 2].WData[ 3] =0x32;
            DataUI[ 2].WData[ 4] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[3 +1];
            DataUI[ 3].WData[ 0] =0x3161;
            DataUI[ 3].WData[ 1] =0x2c;
            DataUI[ 3].WData[ 2] =0x33;
            DataUI[ 3].WData[ 3] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[3 +1];
            DataUI[ 4].WData[ 0] =0x3131;
            DataUI[ 4].WData[ 1] =0x2c;
            DataUI[ 4].WData[ 2] =0x34;
            DataUI[ 4].WData[ 3] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[3 +1];
            DataUI[ 5].WData[ 0] =0x3134;
            DataUI[ 5].WData[ 1] =0x2c;
            DataUI[ 5].WData[ 2] =0x35;
            DataUI[ 5].WData[ 3] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[3 +1];
            DataUI[ 6].WData[ 0] =0x3137;
            DataUI[ 6].WData[ 1] =0x2c;
            DataUI[ 6].WData[ 2] =0x36;
            DataUI[ 6].WData[ 3] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[4 +1];
            DataUI[ 7].WData[ 0] =0x3139;
            DataUI[ 7].WData[ 1] =0x2c;
            DataUI[ 7].WData[ 2] =0x35;
            DataUI[ 7].WData[ 3] =0x35;
            DataUI[ 7].WData[ 4] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[4 +1];
            DataUI[ 8].WData[ 0] =0x3141;
            DataUI[ 8].WData[ 1] =0x2c;
            DataUI[ 8].WData[ 2] =0x30;
            DataUI[ 8].WData[ 3] =0x30;
            DataUI[ 8].WData[ 4] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[3 +1];
            DataUI[ 9].WData[ 0] =0x3142;
            DataUI[ 9].WData[ 1] =0x2c;
            DataUI[ 9].WData[ 2] =0x37;
            DataUI[ 9].WData[ 3] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[3 +1];
            DataUI[10].WData[ 0] =0x3145;
            DataUI[10].WData[ 1] =0x2c;
            DataUI[10].WData[ 2] =0x38;
            DataUI[10].WData[ 3] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[3 +1];
            DataUI[11].WData[ 0] =0x3147;
            DataUI[11].WData[ 1] =0x2c;
            DataUI[11].WData[ 2] =0x30;
            DataUI[11].WData[ 3] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[3 +1];
            DataUI[12].WData[ 0] =0x3148;
            DataUI[12].WData[ 1] =0x2c;
            DataUI[12].WData[ 2] =0x39;
            DataUI[12].WData[ 3] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[4 +1];
            DataUI[13].WData[ 0] =0x314a;
            DataUI[13].WData[ 1] =0x2c;
            DataUI[13].WData[ 2] =0x39;
            DataUI[13].WData[ 3] =0x39;
            DataUI[13].WData[ 4] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[4 +1];
            DataUI[14].WData[ 0] =0x314b;
            DataUI[14].WData[ 1] =0x2c;
            DataUI[14].WData[ 2] =0x34;
            DataUI[14].WData[ 3] =0x34;
            DataUI[14].WData[ 4] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[4 +1];
            DataUI[15].WData[ 0] =0x314c;
            DataUI[15].WData[ 1] =0x2c;
            DataUI[15].WData[ 2] =0x36;
            DataUI[15].WData[ 3] =0x36;
            DataUI[15].WData[ 4] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[4 +1];
            DataUI[16].WData[ 0] =0x314d;
            DataUI[16].WData[ 1] =0x2c;
            DataUI[16].WData[ 2] =0x37;
            DataUI[16].WData[ 3] =0x37;
            DataUI[16].WData[ 4] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[4 +1];
            DataUI[17].WData[ 0] =0x314e;
            DataUI[17].WData[ 1] =0x2c;
            DataUI[17].WData[ 2] =0x38;
            DataUI[17].WData[ 3] =0x38;
            DataUI[17].WData[ 4] =0;

            //------------------------------------------------------
            // Index 18
            //------------------------------------------------------
            DataUI[18].WData =new wchar_t[5 +1];
            DataUI[18].WData[ 0] =0x3132;
            DataUI[18].WData[ 1] =0x2c;
            DataUI[18].WData[ 2] =0x34;
            DataUI[18].WData[ 3] =0x34;
            DataUI[18].WData[ 4] =0x34;
            DataUI[18].WData[ 5] =0;

            //------------------------------------------------------
            // Index 19
            //------------------------------------------------------
            DataUI[19].WData =new wchar_t[5 +1];
            DataUI[19].WData[ 0] =0x3138;
            DataUI[19].WData[ 1] =0x2c;
            DataUI[19].WData[ 2] =0x36;
            DataUI[19].WData[ 3] =0x36;
            DataUI[19].WData[ 4] =0x36;
            DataUI[19].WData[ 5] =0;

            //------------------------------------------------------
            // Index 20
            //------------------------------------------------------
            DataUI[20].WData =new wchar_t[5 +1];
            DataUI[20].WData[ 0] =0x3143;
            DataUI[20].WData[ 1] =0x2c;
            DataUI[20].WData[ 2] =0x37;
            DataUI[20].WData[ 3] =0x37;
            DataUI[20].WData[ 4] =0x37;
            DataUI[20].WData[ 5] =0;

            //------------------------------------------------------
            // Index 21
            //------------------------------------------------------
            DataUI[21].WData =new wchar_t[5 +1];
            DataUI[21].WData[ 0] =0x3146;
            DataUI[21].WData[ 1] =0x2c;
            DataUI[21].WData[ 2] =0x38;
            DataUI[21].WData[ 3] =0x38;
            DataUI[21].WData[ 4] =0x38;
            DataUI[21].WData[ 5] =0;

            //------------------------------------------------------
            // Index 22
            //------------------------------------------------------
            DataUI[22].WData =new wchar_t[5 +1];
            DataUI[22].WData[ 0] =0x3149;
            DataUI[22].WData[ 1] =0x2c;
            DataUI[22].WData[ 2] =0x39;
            DataUI[22].WData[ 3] =0x39;
            DataUI[22].WData[ 4] =0x39;
            DataUI[22].WData[ 5] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
