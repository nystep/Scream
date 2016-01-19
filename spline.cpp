#if 0
void spline(double control[MAX_VERTICES][3])
{
  int i,k;
  double u;

  selectColor(RED);
  glLineWidth(LINE_WIDTH_2);

  for (k=1; k<(numVerticesControl-2); k++)
    {
      for (i=0; i<maxVerticesCurve; i++)
	{
	  u = (double)i / (double)(maxVerticesCurve-1);
	  spline2D[maxVerticesCurve*(k-1)+i][0] = (2*u*u*u - 3*u*u + 1)*control[k][0] 
	    + (-2*u*u*u + 3*u*u)*control[k+1][0]
	    + (u*u*u - 2*u*u + u)*(0.5*(1-tension)*((1+bias)*(1-continuity)*(control[k][0]-control[k-1][0])
						    + (1-bias)*(1+continuity)*(control[k+1][0]-control[k][0])))
	    + (u*u*u - u*u)*(0.5*(1-tension)*((1+bias)*(1+continuity)*(control[k+1][0]-control[k][0])
					      + (1-bias)*(1-continuity)*(control[k+2][0]-control[k+1][0])));
	  spline2D[maxVerticesCurve*(k-1)+i][1] = (2*u*u*u - 3*u*u + 1)*control[k][1] 
	    + (-2*u*u*u + 3*u*u)*control[k+1][1]
	    + (u*u*u - 2*u*u + u)*(0.5*(1-tension)*((1+bias)*(1-continuity)*(control[k][1]-control[k-1][1])
						    + (1-bias)*(1+continuity)*(control[k+1][1]-control[k][1])))
	    + (u*u*u - u*u)*(0.5*(1-tension)*((1+bias)*(1+continuity)*(control[k+1][1]-control[k][1])
					      + (1-bias)*(1-continuity)*(control[k+2][1]-control[k+1][1])));
	  spline2D[maxVerticesCurve*(k-1)+i][2] = (2*u*u*u - 3*u*u + 1)*control[k][2] 
	    + (-2*u*u*u + 3*u*u)*control[k+1][2]
	    + (u*u*u - 2*u*u + u)*(0.5*(1-tension)*((1+bias)*(1-continuity)*(control[k][2]-control[k-1][2])
						    + (1-bias)*(1+continuity)*(control[k+1][2]-control[k][2])))
	    + (u*u*u - u*u)*(0.5*(1-tension)*((1+bias)*(1+continuity)*(control[k+1][2]-control[k][2])
					      + (1-bias)*(1-continuity)*(control[k+2][2]-control[k+1][2])));
	}
    }

  /* draw the Spline curve */
  glBegin(GL_LINE_STRIP);
  for (i=0; i<((numVerticesControl-3)*maxVerticesCurve); i++)
    glVertex2f(spline2D[i][0],DEFAULT_WINDOW_HEIGHT - spline2D[i][1]);
  glEnd();

  glLineWidth(DEFAULT_LINE_WIDTH);
}
#endif
