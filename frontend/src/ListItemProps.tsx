export interface ListItemProps<T> {
   item: T
   onDelete: (id: number) => void;
}
