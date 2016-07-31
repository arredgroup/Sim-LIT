	unsigned char SetLSB(unsigned char val)
	{
	  val = val | 0x01;
	  return val;
	}

	unsigned char ResetLSB(unsigned char val)
	{
	  val = val & 0xFE;
	  return val;
	}

	unsigned char GetLSB(unsigned char val)
	{
	  val = (val & 0x01);
	  return val;
	}

	int hex2dec (unsigned char *hex, int n) // Conversion d'un nombre codé sur n octets (du poids fort au poids faible) en décimal
	{
		int value = 0;

		for (int i = n - 1; i >= 0; i--)	{  value = value * 256 + hex[i];  }
		return value;
	}

	void dec2hex (int value, unsigned char *hex, int n) // Conversion d'un entier en nombre codé sur n octets (du poids fort au poids faible)
	{	
		for (int i = 0; i < n; i++) hex[i]=0;

		for (int i = 0; i < n; i++)
		   {
	  	     hex[i] =  value % 256;
	  	     value = value / 256;
		   }
	}

	unsigned char WatermarkGet(unsigned char a, unsigned char b, unsigned char c)
	{
	  unsigned char code, position;
	  
	  code=(GetLSB(a) * 4) + (GetLSB(b) * 2) + GetLSB(c);

	  switch (code)
	  	{
		 case 0: position = 0; break;
		 case 7: position = 0; break;
		 case 1: position = 1; break;
		 case 6: position = 1; break;	 
		 case 2: position = 2; break;
		 case 5: position = 2; break;
		 case 3: position = 3; break;
		 case 4: position = 3; break;
		}
	  return position;
	}

	void WatermarkSet(unsigned char &a, unsigned char &b, unsigned char &c, int position)
	{
	  unsigned char bit1, bit2, bit3, code;
	  
	  code=(GetLSB(a) * 4) + (GetLSB(b) * 2) + GetLSB(c);

	  switch (position)
	      	{
		 /* On doit tatouer que le pixel effacé est x0 */
		 case 0:
		 	switch (code)
			      {
				case 1: c=ResetLSB(c);	break;
				case 2: b=ResetLSB(b);	break;
				case 3: a=SetLSB(a);	break;
				case 4: a=ResetLSB(a);	break;
				case 5: b=SetLSB(b);	break;
				case 6: c=SetLSB(c);	break;
			      }	
			break;      

		 /* On doit tatouer que le pixel effacé est x1 */
		 case 1:
		 	switch (code)
			      {
				case 0: c=SetLSB(c);	break;
				case 2: a=SetLSB(a);	break;
				case 3: b=ResetLSB(b);	break;
				case 4: b=SetLSB(b);	break;
				case 5: a=ResetLSB(a);	break;
				case 7: c=ResetLSB(c);	break;
			      }	
			break;      

		 /* On doit tatouer que le pixel effacé est x2 */
		 case 2:
		 	switch (code)
			      {
				case 0: b=SetLSB(b);	break;
				case 1: a=SetLSB(a);	break;
				case 3: c=ResetLSB(c);	break;
				case 4: c=SetLSB(c);	break;
				case 6: a=ResetLSB(a);	break;
				case 7: b=ResetLSB(b);	break;
			      }	
			break;      
	  
		 /* On doit tatouer que le pixel effacé est x3 */
		 case 3:
		 	switch (code)
			      {
				case 0: a=SetLSB(a);	break;
				case 1: b=SetLSB(b);	break;
				case 2: c=SetLSB(c);	break;
				case 5: c=ResetLSB(c);	break;
				case 6: b=ResetLSB(b);	break;
				case 7: a=ResetLSB(a);	break;
			      }	
			break;
		}	
	  return;
	}