import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, JoinColumn, OneToOne } from "typeorm";
import { LoadCase } from "./LoadCase";
import { StructuralPoint } from "./StructuralPoint";

@Entity()
export class PointAction {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @OneToOne(type => StructuralPoint)
   @JoinColumn()
   referencePoint: StructuralPoint;

   @Column()
   value_x: number; // [kN]

   @Column()
   value_y: number; // [kN]

   @Column()
   value_z: number; // [kN]

   @OneToOne(type => LoadCase)
   @JoinColumn()
   loadcase: LoadCase;
}