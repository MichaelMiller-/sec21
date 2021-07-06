import { Entity, PrimaryGeneratedColumn, Column, OneToOne, JoinColumn } from "typeorm";
import { StructuralPoint } from "./StructuralPoint";

@Entity()
export class Displacement {

   @PrimaryGeneratedColumn()
   id: number;

   @Column("real")
   displacement_x: number = 0;

   @Column("real")
   displacement_y: number = 0;

   @Column("real")
   displacement_z: number = 0;

   @OneToOne(type => StructuralPoint)
   @JoinColumn()
   node: StructuralPoint;
}
