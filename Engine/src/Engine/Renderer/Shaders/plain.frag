   #version 410 core
   layout(location = 0) out vec4 fragmentColor;
   in vec3 v_pos;

   void main()
   {
      fragmentColor = vec4(v_pos+0.5, 1.0);
   }