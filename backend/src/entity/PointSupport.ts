import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, JoinColumn, OneToOne } from "typeorm";
import { LoadCase } from "./LoadCase";
import { StructuralPoint } from "./StructuralPoint";

export enum SupportType {
   FIXED = "fixed",
   HINGED = "hinged",
   SLIDING = "sliding",
   CUSTOM = "custom"
}

@Entity()
export class PointSupport {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   @Generated("uuid")
   uuid: string;

   @OneToOne(type => StructuralPoint)
   @JoinColumn()
   referencePoint: StructuralPoint;

   @Column({
      type: "enum",
      enum: SupportType,
      default: SupportType.FIXED
   })
   supportType: SupportType;

   @Column()
   stiffnessX: number;

   @Column()
   stiffnessY: number;

   @Column()
   stiffnessZ: number;

   @Column()
   stiffnessFix: number;

   @Column()
   stiffnessFiy: number;

   @Column()
   stiffnessFiz: number;
}