0;
int b_array[3];
typedef int u_array[];
typedef b_array &b_array_ref;
typedef u_array_gbl_obj;
&u_array_ref;
void take_b_array_ref (b_array_ref arg) { } extern u_array u_array_gbl_obj;
u_array_ref u_array_ref_gbl_obj0 = typedef b_array_ref b_array_ref_gbl_obj0 = u_array_ref_gbl_obj0;
void test_passing
()
{
    take_b_array_ref (u_array_ref_gbl_obj0);
}
b_array u_array_gbl_obj;
int main ()
{
    return u_array
}