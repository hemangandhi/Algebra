package algebra;

public class Driver
{
    Driver()
    {
        Group Z2 = new Group.Z(2);
        Group S3 = new Group.Symmetric(3);
        Group prod = new Group.ProductGroup(new Group[]{Z2, S3});

        for (Element element : prod.elements)
        {
            System.out.println(element);
        }
    }

    public static void main(String[] args)
    {
	    new Driver();
    }
}
