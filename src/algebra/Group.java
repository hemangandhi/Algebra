package algebra;

import java.util.*;

abstract class Group
{
    Element[] elements;

    static class Symmetric extends Group
    {
        Symmetric(int n)
        {
            int factorial = 1;
            for (int i = 1; i <= n; i++)
                factorial *= i;

            elements = new Element[factorial];

            for (int i = 0; i < factorial; i++)
            {
                int[] arr = new int[n];
                int key = i;
                boolean[] used = new boolean[n];

                for (int j = n; j >= 1; j--)
                {
                    int next = key % j;
                    key /= j;
                    int count = 0;
                    for (int k = 0; k < n; k++)
                        if (!used[k])
                        {
                            if (count == next)
                            {
                                arr[n - j] = k;
                                used[k] = true;
                                break;
                            }
                            count++;
                        }
                }

                elements[i] = new Element.Permutation(arr);
            }
        }
    }

    static class Z extends Group
    {
        Z(int n)
        {
            elements = new Element[n];

            for (int i = 0; i < n; i++)
                elements[i] = new Element.Int(i, n);
        }
    }

    static class ProductGroup extends Group
    {
        ProductGroup(Group[] groups)
        {
            int count = 1;
            for (Group group : groups)
                count *= group.elements.length;

            elements = new Element[count];

            for (int i = 0; i < count; i++)
            {
                int key = i;
                Element[] next = new Element[groups.length];
                for (int j = 0; j < groups.length; j++)
                {
                    next[j] = groups[j].elements[key % groups[j].elements.length];
                    key /= groups[j].elements.length;
                }

                elements[i] = new Element.Tuple(next);
            }
        }
    }
}
