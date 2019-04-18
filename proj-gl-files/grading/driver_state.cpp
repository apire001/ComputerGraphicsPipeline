#include "driver_state.h"
#include <cstring>

driver_state::driver_state()
{
}

driver_state::~driver_state()
{
    delete [] image_color;
    delete [] image_depth;
}

// This function should allocate and initialize the arrays that store color and
// depth.  This is not done during the constructor since the width and height
// are not known when this class is constructed.
void initialize_render(driver_state& state, int width, int height)
{
    state.image_width=width;
    state.image_height=height;
    state.image_color=new pixel[width*height];
    for(int i = 0; i < (width * height); ++i){
	state.image_color[i] = make_pixel(0,0,0); //set pixel to black
    }
    state.image_depth = new float[state.image_width * state.image_height];
    for(int a = 0; a < state.image_width * state.image_height; ++a){
	state.image_depth[a] = 2;
    }
    //std::cout<<"TODO: allocate and initialize state.image_color and state.image_depth."<<std::endl;
}

// This function will be called to render the data that has been stored in this class.
// Valid values of type are:
//   render_type::triangle - Each group of three vertices corresponds to a triangle.
//   render_type::indexed -  Each group of three indices in index_data corresponds
//                           to a triangle.  These numbers are indices into vertex_data.
//   render_type::fan -      The vertices are to be interpreted as a triangle fan.
//   render_type::strip -    The vertices are to be interpreted as a triangle strip.
void render(driver_state& state, render_type type)
{
    switch(type){
        case render_type::triangle: //triangle
	    //printf("Floats per vertex: %d\n", state.floats_per_vertex);
	    //printf("Max floats per vertex: %d\n", MAX_FLOATS_PER_VERTEX);
	    int tempint;
	    data_geometry* geo;
	    for(int i = 0; (i < state.num_vertices);){
			const data_geometry* temp[3];
			for(int j = 0; j < 3; ++j){
			    geo = new data_geometry;
			    geo->data = new float[state.floats_per_vertex];
			    for(int k = 0; k < state.floats_per_vertex; ++k){
					tempint = state.floats_per_vertex * i + k;
					//printf("Tempint is %d\n", tempint);
					geo->data[k] = state.vertex_data[tempint];
					//printf("geo.data[%d] is %f\n", k, geo.data[k]);
			    }
			    temp[j] = geo;
			    data_vertex v1;
			    v1.data = geo->data;
			    const data_vertex v2 = v1;
			    state.vertex_shader(v2, *(geo), state.uniform_data);
			    //delete geo;
			    ++i;
			}
			rasterize_triangle(state, temp);
			delete []temp[0]->data;
	    		delete []temp[1]->data;
	    		delete []temp[2]->data;	
	    }
	    break;
	case render_type::indexed: //indexed
	    //printf("Floats per vertex: %d\n", state.floats_per_vertex);
	    //printf("Max floats per vertex: %d\n", MAX_FLOATS_PER_VERTEX);
	    int tempint2;
	    //data_geometry* geo;
	    //printf("Called indexed, num triangles is %d\n", state.num_triangles);
	    for(int i = 0; (i < state.num_triangles);++i){
			const data_geometry* temp[3];
			for(int j = 0; j < 3; ++j){
			    geo = new data_geometry;
			    geo->data = new float[state.floats_per_vertex];
			    tempint2 = state.index_data[i*3 + j] * 3;
			    for(int k = 0; k < state.floats_per_vertex; ++k){
					tempint = tempint2 + k;
					//printf("Tempint is %d\n", tempint);
					geo->data[k] = state.vertex_data[tempint];
					//printf("geo.data[%d] is %f\n", k, geo->data[k]);
			    }
			    temp[j] = geo;
			    data_vertex v1;
			    v1.data = geo->data;
			    const data_vertex v2 = v1;
			    state.vertex_shader(v2, *(geo), state.uniform_data);
			    //delete geo;
			}
			rasterize_triangle(state, temp);
			delete []temp[0]->data;
	    		delete []temp[1]->data;
	    		delete []temp[2]->data;	
	    }
	    break;
	case render_type::fan: //fan
	    //printf("Floats per vertex: %d\n", state.floats_per_vertex);
	    //printf("Max floats per vertex: %d\n", MAX_FLOATS_PER_VERTEX);
	    int tempint1;
	    //data_geometry* geo;
	    for(int i = 0; (i+2 < state.num_vertices);++i){
			const data_geometry* temp[3];
			for(int j = 0; j < 3; ++j){
			    geo = new data_geometry;
			    geo->data = new float[state.floats_per_vertex];
			    if(j==0){
				tempint = 0;
			    }
			    else if(j==1){
				tempint = state.floats_per_vertex * (i+1);
			    }
			    else{
				tempint = state.floats_per_vertex * (i+2);
			    }
			    for(int k = 0; k < state.floats_per_vertex; ++k){
					tempint1 = tempint + k;
					//printf("Tempint is %d\n", tempint);
					geo->data[k] = state.vertex_data[tempint1];
					//printf("geo.data[%d] is %f\n", k, geo.data[k]);
			    }
			    temp[j] = geo;
			    data_vertex v1;
			    v1.data = geo->data;
			    const data_vertex v2 = v1;
			    state.vertex_shader(v2, *(geo), state.uniform_data);
			    //delete geo;
			}
			rasterize_triangle(state, temp);
			delete []temp[0]->data;
	    		delete []temp[1]->data;
	    		delete []temp[2]->data;	
	    }
	    break;
	case render_type::strip: //strip
	    //printf("Floats per vertex: %d\n", state.floats_per_vertex);
	    //printf("Max floats per vertex: %d\n", MAX_FLOATS_PER_VERTEX);
	    //int tempint1;
	    //data_geometry* geo;
	    for(int i = 0; (i+2 < state.num_vertices);++i){
			const data_geometry* temp[3];
			for(int j = 0; j < 3; ++j){
			    geo = new data_geometry;
			    geo->data = new float[state.floats_per_vertex];
			if(i%2 == 1){
			    //printf("is odd\n");
			    if(j==0){
				tempint = i * state.floats_per_vertex;
			    }
			    else if(j==1){
				tempint = state.floats_per_vertex * (i+1);
			    }
			    else{
				tempint = state.floats_per_vertex * (i+2);
			    }
			}
			else{
			    //printf("is even\n");
			    if(j==1){
				tempint = i * state.floats_per_vertex;
			    }
			    else if(j==0){
				tempint = state.floats_per_vertex * (i+1);
			    }
			    else{
				tempint = state.floats_per_vertex * (i+2);
			    }
			}
			    for(int k = 0; k < state.floats_per_vertex; ++k){
					tempint1 = tempint + k;
					//printf("Tempint is %d\n", tempint);
					geo->data[k] = state.vertex_data[tempint1];
					//printf("geo.data[%d] is %f\n", k, geo.data[k]);
			    }
			    temp[j] = geo;
			    data_vertex v1;
			    v1.data = geo->data;
			    const data_vertex v2 = v1;
			    state.vertex_shader(v2, *(geo), state.uniform_data);
			    //delete geo;
			}
			rasterize_triangle(state, temp);
			delete []temp[0]->data;
	    		delete []temp[1]->data;
	    		delete []temp[2]->data;	
	    }
	    break;
	default:
	    break;
    }
}


// This function clips a triangle (defined by the three vertices in the "in" array).
// It will be called recursively, once for each clipping face (face=0, 1, ..., 5) to
// clip against each of the clipping faces in turn.  When face=6, clip_triangle should
// simply pass the call on to rasterize_triangle.
void clip_triangle(driver_state& state, const data_geometry* in[3], int face)
{
    if(face==6)
    {
        rasterize_triangle(state, in);
        return;
    }
    //std::cout<<"TODO: implement clipping. (The current code passes the triangle through without clipping them.)"<<std::endl;
    clip_triangle(state,in,face+1);
}

float max(float a, float b){
    if(a > b){
	return a;
    }
    else{
	return b;
    }
}

float min(float a, float b){
    if(a > b){
	return b;
    }
    else{
	return a;
    }
}


// Rasterize the triangle defined by the three vertices in the "in" array.  This
// function is responsible for rasterization, interpolation of data to
// fragments, calling the fragment shader, and z-buffering.
void rasterize_triangle(driver_state& state, const data_geometry* in[3])
{
    vec4 vertices[3];
    vec4 pixel_coordinates;
    for(int i = 0; i < 3; ++i){
		//printf("in[%d]->gl_position is %f, %f, %f, %f\n", i, in[i]->gl_Position[0], in[i]->gl_Position[1], in[i]->gl_Position[2], in[i]->gl_Position[3]);
		pixel_coordinates = in[i]->gl_Position;
		pixel_coordinates = pixel_coordinates / in[i]->gl_Position[3];
		pixel_coordinates[0] = pixel_coordinates[0] * state.image_width * 0.5 + state.image_width * 0.5 - 0.5;
		pixel_coordinates[1] = pixel_coordinates[1] * state.image_height * 0.5 + state.image_height * 0.5 - 0.5;
		vertices[i] = pixel_coordinates;
		vertices[i][3] = in[i]->gl_Position[3];
    }
    float px;
    float py;
    float ax = vertices[0][0];
    float ay = vertices[0][1];
    float bx = vertices[1][0];
    float by = vertices[1][1];
    float cx = vertices[2][0];
    float cy = vertices[2][1];
    //printf("ax: %f ay: %f\n", ax, ay);
    //printf("bx: %f by: %f\n", bx, by);
    //printf("cx: %f cy: %f\n", cx, cy);
    double areaABC = 0.5*(ax*by+bx*cy+cx*ay-ay*bx-by*cx-cy*ax);
    //printf("areaABC: %f\n", areaABC);
    double areaPBC;
    double areaPCA;
    double alpha;
    double beta;
    double gamma;
    double alpha_prime;
    double beta_prime;
    double gamma_prime;
    double k;
    data_fragment inDF;
    data_output out;
    inDF.data = new float[state.floats_per_vertex];
    float z;
    vec3 point;
    vec3 normal;
    vec3 temp1;
    vec3 temp2;
    vec3 temp3;
    double d;
    int minx = min(ax,bx);
    minx = min(minx, cx);
    minx = max(minx, 1);
    int miny = min(ay,by);
    miny = min(miny, cy);
    miny = max(miny, 1);
    int maxx = max(ax,bx);
    maxx = max(maxx,cx);
    maxx = min(maxx, state.image_width - 2);
    int maxy = max(ay,by);
    maxy = max(maxy,cy);
    maxy = min(maxy, state.image_height - 2);
    //printf("before for loop\n");
    for(int x = minx - 1; x < maxx + 1; ++x){
        for(int y = miny - 1; y < maxy + 1; ++y){
	//if((x >= minx)&&(x <= maxx)&&(y >= miny)&&(y <= maxy)){
	    px = x;
	    py = y; 
            areaPBC = 0.5*(px*by+bx*cy+cx*py-py*bx-by*cx-cy*px);
	    //printf("ax: %f ay: %f\n", ax, ay);
            //printf("bx: %f by: %f\n", bx, by);
            //printf("cx: %f cy: %f\n", cx, cy);
	    //printf("px: %f py: %f\n", px, py);
	    //printf("areaPBC: %f\n", areaPBC);
            areaPCA = 0.5*(px*cy+cx*ay+ax*py-py*cx-cy*ax-ay*px);
	    //printf("areaPCA: %f\n", areaPCA);
            alpha = areaPBC / areaABC;
            beta = areaPCA / areaABC;
       	    gamma = 1.0 - alpha - beta;
	    //printf("Alpha: %f Beta: %f Gamma: %f\n", alpha, beta, gamma);
	    if((alpha >= -0.0001) && (beta >= -0.0001) && (gamma >= -0.0001)){
		//interpolate z, check if nearest object at point in z-space
		temp1[0] = vertices[0][0];
		temp1[1] = vertices[0][1];
		temp1[2] = vertices[0][2];
		temp2[0] = vertices[1][0];
		temp2[1] = vertices[1][1];
		temp2[2] = vertices[1][2];
		temp3[0] = vertices[2][0];
		temp3[1] = vertices[2][1];
		temp3[2] = vertices[2][2];
		temp1 = temp2 - temp1;
		temp2 = temp3 - temp2;
		point = temp3;
		normal = cross(temp1, temp2);
		d = normal[0]*point[0] + normal[1]*point[1] + normal[2]*point[2];
		z = d;
		z = z - (px*normal[0]) - (py*normal[1]);
		z = z / normal[2];
		//printf("z is: %f\n", z);
		//printf("before z check\n");
		if((z < state.image_depth[x + y * state.image_width])&&(z > -1.0)&&(z < 1.0)){
		    //printf("after z check\n");
		    //calculate new color and set pixel color
		    alpha_prime = alpha;
		    beta_prime = beta;
		    gamma_prime = gamma;
		    k = (alpha_prime / vertices[0][3]) + (beta_prime / vertices[1][3]) + (gamma_prime / vertices[2][3]);
		    alpha = (alpha_prime / vertices[0][3]) / k;
		    beta = (beta_prime / vertices[1][3]) / k;
		    gamma = (gamma_prime / vertices[2][3]) / k;
		    for(int i = 0; i < state.floats_per_vertex; ++i){
		        switch(state.interp_rules[i]){
			    case interp_type::flat:
			        inDF.data[i] = in[0]->data[i];
			        break;
			    case interp_type::smooth:
			        inDF.data[i] = alpha * in[0]->data[i] + beta * in[1]->data[i] + gamma * in[2]->data[i];
			        break;
			    case interp_type::noperspective:
			        inDF.data[i] = alpha_prime * in[0]->data[i] + beta_prime * in[1]->data[i] + gamma_prime * in[2]->data[i];
			        break;
			    case interp_type::invalid:
				break;
		        }
		    }
		    state.fragment_shader(inDF, out, state.uniform_data);
		    state.image_color[x + y * state.image_width] = make_pixel(out.output_color[0] * 255, out.output_color[1] * 255, out.output_color[2] * 255);
		    state.image_depth[x + y * state.image_width] = z;
		}
	    //}
	    }
	}
    }
    delete []inDF.data;
    //std::cout<<"TODO: implement rasterization"<<std::endl;
}

