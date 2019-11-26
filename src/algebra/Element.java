package algebra;

abstract class Element
{
    abstract Element multiply(Element other);

    class Permutation extends Element
    {
        int[] arr;

        Permutation(int[] in) { arr = in; }

        @Override
        Element multiply(Element in)
        {
            if (!(in instanceof Permutation))
            {
                System.out.println("Multiplication Type Error");
                return null;
            }

            Permutation other = (Permutation) in;

            if (arr.length != other.arr.length)
            {
                System.out.println("Permutation Size Error");
                return null;
            }

            int[] out = new int[arr.length];
            for (int i = 0; i < arr.length; i++)
                out[i] = arr[other.arr[i]];

            return new Permutation(out);
        }
    }
}